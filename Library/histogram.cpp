#include "histogram.h"
#include "common.h"

#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <memory>
#include <vector>

class HistogramCanvas final : public QWidget
{
public:
    explicit HistogramCanvas(const Histogram::Base *histogram, QWidget *parent =
            nullptr) : QWidget(parent), histogram_(histogram)
    {
        setMinimumSize(640, 480);
        setAutoFillBackground(true);
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(252, 252, 252));
        setPalette(palette);
    }

    void SetParameter(float parameter)
    {
        parameter_ = parameter;
        update();
    }

protected:
    void paintEvent(QPaintEvent* /*event*/) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        const QRect plot_area = PlotArea();
        DrawBackground(painter);
        DrawGrid(painter, plot_area);
        DrawAxes(painter, plot_area);
        DrawHistograms(painter, plot_area);
        DrawLegend(painter, plot_area);
    }

private:
    QRect PlotArea() const
    {
        return rect().adjusted(kPlotMarginLeft, kPlotMarginTop,
                -kPlotMarginRight, -kPlotMarginBottom);
    }

    float ToPixelX(const QRect &plot_area, float x) const
    {
        const float x_range = histogram_->MaxX - histogram_->MinX;
        const float x_ratio =
                x_range == 0.0f ? 0.0f : (x - histogram_->MinX) / x_range;
        return plot_area.left() + x_ratio * plot_area.width();
    }

    float ToPixelY(const QRect &plot_area, float y) const
    {
        const float y_range = histogram_->MaxY - histogram_->MinY;
        const float y_ratio =
                y_range == 0.0f ? 0.0f : (y - histogram_->MinY) / y_range;
        return plot_area.bottom() - y_ratio * plot_area.height();
    }

    void DrawBackground(QPainter &painter) const
    {
        painter.fillRect(rect(), QColor(252, 252, 252));
        painter.fillRect(PlotArea(), Qt::white);
    }

    void DrawGrid(QPainter &painter, const QRect &plot_area) const
    {
        QPen grid_pen(QColor(220, 220, 220));
        grid_pen.setStyle(Qt::DotLine);
        painter.setPen(grid_pen);

        constexpr int kGridLines = 8;
        for (int i = 1; i < kGridLines; ++i)
        {
            const int x = plot_area.left()
                    + (plot_area.width() * i) / kGridLines;
            const int y = plot_area.top()
                    + (plot_area.height() * i) / kGridLines;
            painter.drawLine(x, plot_area.top(), x, plot_area.bottom());
            painter.drawLine(plot_area.left(), y, plot_area.right(), y);
        }
    }

    void DrawAxes(QPainter &painter, const QRect &plot_area) const
    {
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawRect(plot_area);

        painter.setPen(Qt::black);
        const QRect x_label_rect(plot_area.left(), rect().bottom() - 24,
                                 plot_area.width(), 20);
        painter.drawText(x_label_rect, Qt::AlignHCenter | Qt::AlignBottom,
                         QString::fromStdString(histogram_->XLabel));
        painter.save();
        painter.translate(18, plot_area.center().y());
        painter.rotate(-90);
        painter.drawText(-50, -10, 100, 20, Qt::AlignHCenter | Qt::AlignVCenter,
                         QString::fromStdString(histogram_->YLabel));
        painter.restore();
    }

    void DrawHistograms(QPainter &painter, const QRect &plot_area) const
    {
        const std::vector<std::unique_ptr<Histogram::Data>> &data_sets =
                histogram_->DataSets();
        if (data_sets.empty()) return;

        const std::size_t data_set_count = data_sets.size();
        const float baseline_y = ToPixelY(plot_area, histogram_->MinY);

        for (std::size_t data_index = 0; data_index < data_set_count;
                ++data_index)
        {
            const Histogram::Data *data = data_sets[data_index].get();
            if (data == nullptr || data->Size <= 0) continue;

            const QColor color = ParseColor(data->Color);
            const float bin_count = static_cast<float>(data->Size);
            const float bin_width = (histogram_->MaxX - histogram_->MinX)
                    / bin_count;
            const float group_width = bin_width * 0.9f;
            const float bar_width = group_width
                    / static_cast<float>(data_set_count);
            const float group_offset = (static_cast<float>(data_index)
                    - (static_cast<float>(data_set_count) - 1.0f) / 2.0f)
                    * bar_width;

            painter.setPen(QPen(color.darker(120), 1.0));
            painter.setBrush(color);

            for (int bin = 0; bin < data->Size; ++bin)
            {
                const float center = histogram_->MinX
                        + (static_cast<float>(bin) + 0.5f) * bin_width;
                const float count = data->Count(parameter_, bin);
                const float left = ToPixelX(plot_area,
                        center + group_offset - bar_width / 2.0f);
                const float right = ToPixelX(plot_area,
                        center + group_offset + bar_width / 2.0f);
                const float top = ToPixelY(plot_area, count);

                const QRectF bar(std::min(left, right),
                        std::min(top, baseline_y), std::abs(right - left),
                        std::abs(baseline_y - top));
                painter.drawRect(bar);
            }
        }
    }

    void DrawLegend(QPainter &painter, const QRect &plot_area) const
    {
        std::vector<LegendItem> items;
        for (const auto &data : histogram_->DataSets())
        {
            if (data == nullptr) continue;

            items.push_back( { data->Label, ParseColor(data->Color),
                    LegendSwatch::Bar });
        }

        ::DrawLegend(painter, plot_area, items);
    }

    const Histogram::Base *histogram_;
    float parameter_ = 0.0f;
};

class HistogramWindow final : public QWidget
{
public:
    explicit HistogramWindow(const Histogram::Base *histogram) : histogram_(histogram)
    {
        setWindowTitle(QString::fromStdString(histogram->WindowTitle));
        resize(900, 700);

        auto *layout = new QVBoxLayout(this);

        title_label_ = new QLabel(this);
        layout->addWidget(title_label_);

        histogram_canvas_ = new HistogramCanvas(histogram_, this);
        layout->addWidget(histogram_canvas_, 1);

        slider_ = new QSlider(Qt::Horizontal, this);
        slider_->setRange(0, kSliderSteps);
        slider_->setValue(0);
        layout->addWidget(slider_);

        slider_->setVisible(histogram_->Slider);
        if (histogram_->Slider)
        {
            connect(slider_, &QSlider::valueChanged, this, [this](int value)
            {    UpdateDisplay(
                        SliderToParameter(value, histogram_->MinP,
                                histogram_->MaxP));
            });
        }

        UpdateDisplay(histogram_->MinP);
    }

private:
    void UpdateDisplay(float parameter)
    {
        SetTitleLabel(*title_label_,
                histogram_->Slider ?
                        histogram_->Title(parameter) : histogram_->WindowTitle);
        histogram_canvas_->SetParameter(parameter);
    }

    const Histogram::Base *histogram_;
    QLabel *title_label_ = nullptr;
    HistogramCanvas *histogram_canvas_ = nullptr;
    QSlider *slider_ = nullptr;
};

class HistogramViewWindow final : public QWidget
{
public:
    HistogramViewWindow(const Histogram::Base *histogram, float parameter) : histogram_(
            histogram)
    {
        setWindowTitle(QString::fromStdString(histogram_->WindowTitle));
        resize(900, 700);

        auto *layout = new QVBoxLayout(this);

        title_label_ = new QLabel(this);
        SetTitleLabel(*title_label_,
                histogram_->Slider ?
                        histogram_->Title(parameter) : histogram_->WindowTitle);
        layout->addWidget(title_label_);

        canvas_ = new HistogramCanvas(histogram_, this);
        layout->addWidget(canvas_, 1);
        canvas_->SetParameter(parameter);
    }

private:
    const Histogram::Base *histogram_;
    QLabel *title_label_ = nullptr;
    HistogramCanvas *canvas_ = nullptr;
};

void Histogram::Plot(Base& plot)
{
    RunQT(std::make_unique < HistogramWindow > (&plot));
}

void Histogram::Show(Base& plot, const float parameter)
{
    RunQT(std::make_unique < HistogramViewWindow > (&plot, parameter));
}
