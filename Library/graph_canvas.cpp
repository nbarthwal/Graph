#include "graph_impl.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include <vector>

namespace
{

    constexpr std::size_t kSampleCount = 500;

    using plot_detail::kPlotMarginBottom;
    using plot_detail::kPlotMarginLeft;
    using plot_detail::kPlotMarginRight;
    using plot_detail::kPlotMarginTop;
    using plot_detail::ParseColor;

    std::vector<float> Linspace(float min_x, float max_x, std::size_t count)
    {
        std::vector<float> values(count);
        if (count == 0)
        {
            return values;
        }
        if (count == 1)
        {
            values[0] = min_x;
            return values;
        }

        const float step = (max_x - min_x) / static_cast<float>(count - 1);
        for (std::size_t i = 0; i < count; ++i)
        {
            values[i] = min_x + step * static_cast<float>(i);
        }
        return values;
    }

}  // namespace

GraphCanvas::GraphCanvas(Graph &graph, QWidget *parent) :
        QWidget(parent), graph_(graph)
{
    setMinimumSize(640, 480);
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(252, 252, 252));
    setPalette(palette);
}

void GraphCanvas::SetParameter(float parameter)
{
    parameter_ = parameter;
    update();
}

void GraphCanvas::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRect plot_area = PlotArea();
    DrawBackground(painter);
    DrawGrid(painter, plot_area);
    DrawAxes(painter, plot_area);

    for (const Graph::Data *curve : graph_.Curves())
    {
        if (curve != nullptr)
        {
            DrawCurve(painter, plot_area, *curve);
        }
    }
}

QRect GraphCanvas::PlotArea() const
{
    return rect().adjusted(kPlotMarginLeft, kPlotMarginTop, -kPlotMarginRight,
            -kPlotMarginBottom);
}

QPointF GraphCanvas::ToPixel(const QRect &plot_area, float x, float y) const
{
    const float x_range = graph_.MaxX() - graph_.MinX();
    const float y_range = graph_.MaxY() - graph_.MinY();

    const float x_ratio =
            x_range == 0.0f ? 0.0f : (x - graph_.MinX()) / x_range;
    const float y_ratio =
            y_range == 0.0f ? 0.0f : (y - graph_.MinY()) / y_range;

    return QPointF(plot_area.left() + x_ratio * plot_area.width(),
            plot_area.bottom() - y_ratio * plot_area.height());
}

void GraphCanvas::DrawBackground(QPainter &painter) const
{
    painter.fillRect(rect(), QColor(252, 252, 252));
    painter.fillRect(PlotArea(), Qt::white);
}

void GraphCanvas::DrawGrid(QPainter &painter, const QRect &plot_area) const
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

void GraphCanvas::DrawAxes(QPainter &painter, const QRect &plot_area) const
{
    painter.setPen(QPen(Qt::black, 1.5));
    painter.drawRect(plot_area);

    painter.setPen(Qt::black);
    painter.drawText(plot_area.center().x() - 10, rect().bottom() - 12, "x");
    painter.save();
    painter.translate(18, plot_area.center().y());
    painter.rotate(-90);
    painter.drawText(0, 0, "y");
    painter.restore();
}

void GraphCanvas::DrawCurve(QPainter &painter, const QRect &plot_area,
        const Graph::Data &curve) const
{
    const std::vector<float> x_values = Linspace(curve.MinX(), curve.MaxX(),
            kSampleCount);
    if (x_values.empty())
    {
        return;
    }

    const QColor color = ParseColor(curve.Color());

    if (curve.Point())
    {
        QPen pen(color, 1.5);
        painter.setPen(pen);
        painter.setBrush(color);

        constexpr double kPointRadius = 1.25;
        for (const float x : x_values)
        {
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
    for (const float x : x_values)
    {
        const float y = curve.Value(parameter_, x);
        const QPointF point = ToPixel(plot_area, x, y);
        if (!started)
        {
            path.moveTo(point);
            started = true;
        }
        else
        {
            path.lineTo(point);
        }
    }
    painter.drawPath(path);
}
