#include "graph_plot.h"

#include <QApplication>
#include <QColor>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include <cmath>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr int kSliderSteps = 1000;
constexpr std::size_t kSampleCount = 500;
constexpr int kPlotMarginLeft = 60;
constexpr int kPlotMarginRight = 20;
constexpr int kPlotMarginTop = 20;
constexpr int kPlotMarginBottom = 50;

float SliderToParameter(int slider_value, float min_p, float max_p)
{
    const float t = static_cast<float>(slider_value) / static_cast<float>(kSliderSteps);
    return min_p + t * (max_p - min_p);
}

std::vector<float> Linspace(float min_x, float max_x, std::size_t count)
{
    std::vector<float> values(count);
    if (count == 0) {
        return values;
    }
    if (count == 1) {
        values[0] = min_x;
        return values;
    }

    const float step = (max_x - min_x) / static_cast<float>(count - 1);
    for (std::size_t i = 0; i < count; ++i) {
        values[i] = min_x + step * static_cast<float>(i);
    }
    return values;
}

QColor ParseColor(const std::string& color)
{
    static const std::unordered_map<std::string, QColor> named_colors = {
        {"red", QColor(220, 50, 47)},
        {"r", QColor(220, 50, 47)},
        {"blue", QColor(38, 139, 210)},
        {"b", QColor(38, 139, 210)},
        {"green", QColor(133, 153, 0)},
        {"g", QColor(133, 153, 0)},
        {"orange", QColor(203, 75, 22)},
        {"purple", QColor(108, 113, 196)},
        {"black", QColor(0, 0, 0)},
        {"k", QColor(0, 0, 0)},
        {"white", QColor(255, 255, 255)},
        {"w", QColor(255, 255, 255)},
        {"cyan", QColor(42, 161, 152)},
        {"magenta", QColor(211, 54, 130)},
        {"yellow", QColor(181, 137, 0)},
    };

    const auto it = named_colors.find(color);
    if (it != named_colors.end()) {
        return it->second;
    }

    const QColor parsed(QString::fromStdString(color));
    return parsed.isValid() ? parsed : QColor(38, 139, 210);
}

class PlotCanvas final : public QWidget
{
public:
    PlotCanvas(Graph& graph, QWidget* parent = nullptr)
        : QWidget(parent), graph_(graph)
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

        for (const Curve* curve : graph_.Curves()) {
            if (curve != nullptr) {
                DrawCurve(painter, plot_area, *curve);
            }
        }
    }

private:
    QRect PlotArea() const
    {
        return rect().adjusted(
            kPlotMarginLeft, kPlotMarginTop, -kPlotMarginRight, -kPlotMarginBottom);
    }

    QPointF ToPixel(const QRect& plot_area, float x, float y) const
    {
        const float x_range = graph_.MaxX() - graph_.MinX();
        const float y_range = graph_.MaxY() - graph_.MinY();

        const float x_ratio = x_range == 0.0f ? 0.0f : (x - graph_.MinX()) / x_range;
        const float y_ratio = y_range == 0.0f ? 0.0f : (y - graph_.MinY()) / y_range;

        return QPointF(
            plot_area.left() + x_ratio * plot_area.width(),
            plot_area.bottom() - y_ratio * plot_area.height());
    }

    void DrawBackground(QPainter& painter) const
    {
        painter.fillRect(rect(), QColor(252, 252, 252));
        painter.fillRect(PlotArea(), Qt::white);
    }

    void DrawGrid(QPainter& painter, const QRect& plot_area) const
    {
        QPen grid_pen(QColor(220, 220, 220));
        grid_pen.setStyle(Qt::DotLine);
        painter.setPen(grid_pen);

        constexpr int kGridLines = 8;
        for (int i = 1; i < kGridLines; ++i) {
            const int x = plot_area.left() + (plot_area.width() * i) / kGridLines;
            const int y = plot_area.top() + (plot_area.height() * i) / kGridLines;
            painter.drawLine(x, plot_area.top(), x, plot_area.bottom());
            painter.drawLine(plot_area.left(), y, plot_area.right(), y);
        }
    }

    void DrawAxes(QPainter& painter, const QRect& plot_area) const
    {
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawRect(plot_area);

        painter.setPen(Qt::black);
        painter.drawText(
            plot_area.center().x() - 10,
            rect().bottom() - 12,
            "x");
        painter.save();
        painter.translate(18, plot_area.center().y());
        painter.rotate(-90);
        painter.drawText(0, 0, "y");
        painter.restore();
    }

    void DrawCurve(QPainter& painter, const QRect& plot_area, const Curve& curve) const
    {
        const std::vector<float> x_values = Linspace(curve.MinX(), curve.MaxX(), kSampleCount);
        if (x_values.empty()) {
            return;
        }

        const QColor color = ParseColor(curve.Color());

        if (curve.Point()) {
            QPen pen(color, 1.5);
            painter.setPen(pen);
            painter.setBrush(color);

            constexpr double kPointRadius = 1;
            for (const float x : x_values) {
                const float y = curve.Value(parameter_, x);
                const QPointF point = ToPixel(plot_area, x, y);
                painter.drawEllipse(point, kPointRadius, kPointRadius);
            }
            return;
        }

        QPen pen(color, 2.0);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        QPainterPath path;
        bool started = false;
        for (const float x : x_values) {
            const float y = curve.Value(parameter_, x);
            const QPointF point = ToPixel(plot_area, x, y);
            if (!started) {
                path.moveTo(point);
                started = true;
            } else {
                path.lineTo(point);
            }
        }
        painter.drawPath(path);
    }

    Graph& graph_;
    float parameter_ = 0.0f;
};

class PlotWindow final : public QWidget
{
public:
    explicit PlotWindow(Graph& graph)
        : graph_(graph)
    {
        setWindowTitle("Graph Plot");
        resize(900, 700);

        auto* layout = new QVBoxLayout(this);

        parameter_label_ = new QLabel(this);
        layout->addWidget(parameter_label_);

        plot_canvas_ = new PlotCanvas(graph_, this);
        layout->addWidget(plot_canvas_, 1);

        slider_ = new QSlider(Qt::Horizontal, this);
        slider_->setRange(0, kSliderSteps);
        slider_->setValue(0);
        layout->addWidget(slider_);

        connect(slider_, &QSlider::valueChanged, this, [this](int value) {
            const float parameter =
                SliderToParameter(value, graph_.MinP(), graph_.MaxP());
            parameter_label_->setText(
                QString("Parameter p = %1").arg(static_cast<double>(parameter), 0, 'g', 4));
            plot_canvas_->SetParameter(parameter);
        });

        const float initial_parameter = graph_.MinP();
        parameter_label_->setText(
            QString("Parameter p = %1").arg(static_cast<double>(initial_parameter), 0, 'g', 4));
        plot_canvas_->SetParameter(initial_parameter);
    }

private:
    Graph& graph_;
    QLabel* parameter_label_ = nullptr;
    PlotCanvas* plot_canvas_ = nullptr;
    QSlider* slider_ = nullptr;
};

}  // namespace

void Plot(Graph& graph)
{
    const bool owns_application = QApplication::instance() == nullptr;
    std::unique_ptr<QApplication> owned_application;
    int argc = 0;

    if (owns_application) {
        owned_application = std::make_unique<QApplication>(argc, nullptr);
    }

    PlotWindow window(graph);
    window.show();

    if (owns_application) {
        QApplication::exec();
    }
}
