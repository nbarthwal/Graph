#include "graph.h"

#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <vector>

namespace
{

    using plot_detail::kPlotMarginBottom;
    using plot_detail::kPlotMarginLeft;
    using plot_detail::kPlotMarginRight;
    using plot_detail::kPlotMarginTop;
    using plot_detail::kSliderSteps;
    using plot_detail::LegendItem;
    using plot_detail::LegendSwatch;
    using plot_detail::ParseColor;
    using plot_detail::SetTitleLabel;
    using plot_detail::SliderToParameter;

    class HistogramWindow final : public QWidget
    {
    public:
        explicit HistogramWindow(const Histogram &histogram) :
                histogram_(histogram)
        {
            setWindowTitle(QString::fromStdString(histogram.Title()));
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

            const bool show_slider = histogram_.MaxP() != histogram_.MinP();
            slider_->setVisible(show_slider);

            if (show_slider)
            {
                connect(slider_, &QSlider::valueChanged, this,
                        [this](
                                int value)
                                {
                                    UpdateDisplay(
                                            SliderToParameter(value, histogram_.MinP(),
                                                    histogram_.MaxP()));
                                });
            }

            UpdateDisplay(histogram_.MinP());
        }

    private:
        void UpdateDisplay(float parameter)
        {
            SetTitleLabel(*title_label_, histogram_.Title(parameter));
            histogram_canvas_->SetParameter(parameter);
        }

        const Histogram &histogram_;
        QLabel *title_label_ = nullptr;
        HistogramCanvas *histogram_canvas_ = nullptr;
        QSlider *slider_ = nullptr;
    };

    class HistogramViewWindow final : public QWidget
    {
    public:
        HistogramViewWindow(Histogram &histogram, float parameter) :
                histogram_(histogram)
        {
            setWindowTitle(QString::fromStdString(histogram_.Title()));
            resize(900, 700);

            auto *layout = new QVBoxLayout(this);

            title_label_ = new QLabel(this);
            SetTitleLabel(*title_label_, histogram_.Title(parameter));
            layout->addWidget(title_label_);

            canvas_ = new HistogramCanvas(histogram_, this);
            layout->addWidget(canvas_, 1);
            canvas_->SetParameter(parameter);
        }

    private:
        Histogram &histogram_;
        QLabel *title_label_ = nullptr;
        HistogramCanvas *canvas_ = nullptr;
    };

}  // namespace

HistogramCanvas::HistogramCanvas(const Histogram &histogram, QWidget *parent) :
        QWidget(parent), histogram_(histogram)
{
    setMinimumSize(640, 480);
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(252, 252, 252));
    setPalette(palette);
}

void HistogramCanvas::SetParameter(float parameter)
{
    parameter_ = parameter;
    update();
}

void HistogramCanvas::paintEvent(QPaintEvent* /*event*/)
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

QRect HistogramCanvas::PlotArea() const
{
    return rect().adjusted(kPlotMarginLeft, kPlotMarginTop, -kPlotMarginRight,
            -kPlotMarginBottom);
}

float HistogramCanvas::ToPixelX(const QRect &plot_area, float x) const
{
    const float x_range = histogram_.MaxX() - histogram_.MinX();
    const float x_ratio =
            x_range == 0.0f ? 0.0f : (x - histogram_.MinX()) / x_range;
    return plot_area.left() + x_ratio * plot_area.width();
}

float HistogramCanvas::ToPixelY(const QRect &plot_area, float y) const
{
    const float y_range = histogram_.MaxY() - histogram_.MinY();
    const float y_ratio =
            y_range == 0.0f ? 0.0f : (y - histogram_.MinY()) / y_range;
    return plot_area.bottom() - y_ratio * plot_area.height();
}

void HistogramCanvas::DrawBackground(QPainter &painter) const
{
    painter.fillRect(rect(), QColor(252, 252, 252));
    painter.fillRect(PlotArea(), Qt::white);
}

void HistogramCanvas::DrawGrid(QPainter &painter, const QRect &plot_area) const
{
    QPen grid_pen(QColor(220, 220, 220));
    grid_pen.setStyle(Qt::DotLine);
    painter.setPen(grid_pen);

    constexpr int kGridLines = 8;
    for (int i = 1; i < kGridLines; ++i)
    {
        const int x = plot_area.left() + (plot_area.width() * i) / kGridLines;
        const int y = plot_area.top() + (plot_area.height() * i) / kGridLines;
        painter.drawLine(x, plot_area.top(), x, plot_area.bottom());
        painter.drawLine(plot_area.left(), y, plot_area.right(), y);
    }
}

void HistogramCanvas::DrawAxes(QPainter &painter, const QRect &plot_area) const
{
    painter.setPen(QPen(Qt::black, 1.5));
    painter.drawRect(plot_area);

    painter.setPen(Qt::black);
    painter.drawText(plot_area.center().x() - 10, rect().bottom() - 12, "bin");
    painter.save();
    painter.translate(18, plot_area.center().y());
    painter.rotate(-90);
    painter.drawText(0, 0, "count");
    painter.restore();
}

void HistogramCanvas::DrawHistograms(QPainter &painter,
        const QRect &plot_area) const
{
    const std::vector<const Histogram::Data*> data_sets = histogram_.DataSets();
    if (data_sets.empty())
    {
        return;
    }

    const std::size_t data_set_count = data_sets.size();
    const float baseline_y = ToPixelY(plot_area, histogram_.MinY());

    for (std::size_t data_index = 0; data_index < data_set_count; ++data_index)
    {
        const Histogram::Data *data = data_sets[data_index];
        if (data == nullptr || data->BinCount() == 0)
        {
            continue;
        }

        const QColor color = ParseColor(data->Color());
        const float group_width = data->BinWidth() * 0.9f;
        const float bar_width = group_width
                / static_cast<float>(data_set_count);
        const float group_offset = (static_cast<float>(data_index)
                - (static_cast<float>(data_set_count) - 1.0f) / 2.0f)
                * bar_width;

        painter.setPen(QPen(color.darker(120), 1.0));
        painter.setBrush(color);

        for (std::size_t bin = 0; bin < data->BinCount(); ++bin)
        {
            const float center = data->BinCenter(bin);
            const float count = data->Count(parameter_, bin);
            const float left = ToPixelX(plot_area,
                    center + group_offset - bar_width / 2.0f);
            const float right = ToPixelX(plot_area,
                    center + group_offset + bar_width / 2.0f);
            const float top = ToPixelY(plot_area, count);

            const QRectF bar(std::min(left, right), std::min(top, baseline_y),
                    std::abs(right - left), std::abs(baseline_y - top));
            painter.drawRect(bar);
        }
    }
}

void HistogramCanvas::DrawLegend(QPainter &painter,
        const QRect &plot_area) const
{
    std::vector<LegendItem> items;
    for (const Histogram::Data *data : histogram_.DataSets())
    {
        if (data == nullptr)
        {
            continue;
        }

        items.push_back(
                { data->Label(), ParseColor(data->Color()), LegendSwatch::Bar });
    }

    plot_detail::DrawLegend(painter, plot_area, items);
}

void Plot(const Histogram& histogram)
{
    plot_detail::RunQtApp([&histogram]()
    {   return std::make_unique<HistogramWindow>(histogram);});
}

void Histogram::Show(const float parameter)
{
    plot_detail::RunQtApp([this, parameter]()
    {
        return std::make_unique<HistogramViewWindow>(*this, parameter);
    });
}
