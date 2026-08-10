#pragma once

#include <cstddef>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(__CYGWIN__)
#  ifdef GRAPH_PLOT_BUILD_DLL
#    define GRAPH_API __declspec(dllexport)
#  else
#    define GRAPH_API
#  endif
#else
#  define GRAPH_API
#endif

class Graph
{
public:
    class Data
    {
    public:
        virtual float MaxX() const = 0; // MaxX defines the max of X-Axis for this curve.
        virtual float MinX() const = 0; // MinX defines the min of X-Axis for this curve.
        virtual std::string Color() const = 0; // Color defines the color of the curve.
        virtual std::string Label() const = 0; // Label defines the legend label for this curve.
        virtual float Value(float p, float x) const = 0; // Value(p, x) returns y at x for slider value p.
        virtual bool Point() const = 0; // Point defines if the curve should be plotted as a line or point.
        virtual ~Data() = default;
    };

    virtual float MaxX() const = 0; // MaxX defines the max of X-Axis for this graph.
    virtual float MinX() const = 0; // MinX defines the min of X-Axis for this graph.
    virtual float MaxY() const = 0; // MaxY defines the max of Y-Axis for this graph.
    virtual float MinY() const = 0; // MinY defines the min of Y-Axis for this graph.
    virtual float MaxP() const = 0; // MaxP defines the max of slider parameter.
    virtual float MinP() const = 0; // MinP defines the min of slider parameter.
    virtual std::vector<const Data*> Curves() const = 0; // Curves defines the curves for this graph.
    virtual std::string Title() const = 0; // Title defines the window title for this graph.
    virtual std::string Title(const float parameter) const = 0; // Title defines the graph title for slider value p.
    GRAPH_API void Show(const float parameter);
    virtual ~Graph() = default;
};

class Histogram
{
public:
    class Data
    {
    public:
        virtual std::string Color() const = 0; // Color defines the fill color of the bars.
        virtual std::string Label() const = 0; // Label defines the legend label for this data set.
        virtual std::size_t BinCount() const = 0; // Number of bins in this histogram.
        virtual float BinCenter(std::size_t bin) const = 0; // Center of the bin on the x-axis.
        virtual float BinWidth() const = 0;  // Width of each bin on the x-axis.
        virtual float Count(float p, std::size_t bin) const = 0; // Count in bin for slider value p.
        virtual ~Data() = default;
    };

    virtual float MaxX() const = 0; // MaxX defines the max of X-Axis for this histogram.
    virtual float MinX() const = 0; // MinX defines the min of X-Axis for this histogram.
    virtual float MaxY() const = 0; // MaxY defines the max of Y-Axis for this histogram.
    virtual float MinY() const = 0; // MinY defines the min of Y-Axis for this histogram.
    virtual float MaxP() const = 0; // MaxP defines the max of slider parameter.
    virtual float MinP() const = 0; // MinP defines the min of slider parameter.
    virtual std::vector<const Data*> DataSets() const = 0; // Histogram data to display.
    virtual std::string Title() const = 0; // Title defines the window title for this histogram.
    virtual std::string Title(const float parameter) const = 0; // Title defines the histogram title for slider value p.
    GRAPH_API void Show(const float parameter);
    virtual ~Histogram() = default;
};

GRAPH_API void Plot(const Graph& graph);
GRAPH_API void Plot(const Histogram& histogram);

#include "graph_impl.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPen>

inline std::vector<float> GraphLinspace(float min_x, float max_x,
        std::size_t count)
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

class GraphCanvas final : public QWidget
{
public:
    explicit GraphCanvas(const Graph &graph, QWidget *parent = nullptr) :
            QWidget(parent), graph_(graph)
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

        for (const Graph::Data *curve : graph_.Curves())
        {
            if (curve != nullptr)
            {
                DrawCurve(painter, plot_area, *curve);
            }
        }
        DrawLegend(painter, plot_area);
    }

private:
    QRect PlotArea() const
    {
        return rect().adjusted(plot_detail::kPlotMarginLeft,
                plot_detail::kPlotMarginTop, -plot_detail::kPlotMarginRight,
                -plot_detail::kPlotMarginBottom);
    }

    QPointF ToPixel(const QRect &plot_area, float x, float y) const
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
            const int x = plot_area.left() + (plot_area.width() * i) / kGridLines;
            const int y = plot_area.top() + (plot_area.height() * i) / kGridLines;
            painter.drawLine(x, plot_area.top(), x, plot_area.bottom());
            painter.drawLine(plot_area.left(), y, plot_area.right(), y);
        }
    }

    void DrawAxes(QPainter &painter, const QRect &plot_area) const
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

    void DrawCurve(QPainter &painter, const QRect &plot_area,
            const Graph::Data &curve) const
    {
        constexpr std::size_t kSampleCount = 500;
        const std::vector<float> x_values = GraphLinspace(curve.MinX(),
                curve.MaxX(), kSampleCount);
        if (x_values.empty())
        {
            return;
        }

        const QColor color = plot_detail::ParseColor(curve.Color());

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

    void DrawLegend(QPainter &painter, const QRect &plot_area) const
    {
        std::vector<plot_detail::LegendItem> items;
        for (const Graph::Data *curve : graph_.Curves())
        {
            if (curve == nullptr)
            {
                continue;
            }

            items.push_back(
                    { curve->Label(), plot_detail::ParseColor(curve->Color()),
                            curve->Point() ? plot_detail::LegendSwatch::Point
                                           : plot_detail::LegendSwatch::Line });
        }

        plot_detail::DrawLegend(painter, plot_area, items);
    }

    const Graph &graph_;
    float parameter_ = 0.0f;
};
