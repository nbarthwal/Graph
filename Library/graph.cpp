#include "graph.h"
#include "graph_impl.h"
#include <memory>

#include <memory>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPen>

inline std::vector<float> GraphLinspace(float min_x, float max_x,
        std::size_t count)
{
    std::vector<float> values(count);
    if (count == 0) return values;

    if (count == 1)
    {
        values[0] = min_x;
        return values;
    }

    const float step = (max_x - min_x) / static_cast<float>(count - 1);
    for (std::size_t i = 0; i < count; ++i)
        values[i] = min_x + step * static_cast<float>(i);

    return values;
}

class GraphCanvas final : public QWidget
{
public:
    explicit GraphCanvas(const Graph *graph, QWidget *parent = nullptr) : QWidget(
            parent), graph_(graph)
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

        for (const auto &curve : graph_->Segments)
            if (curve != nullptr) DrawCurve(painter, plot_area, *curve);
        DrawLegend(painter, plot_area);
    }

private:
    QRect PlotArea() const
    {
        return rect().adjusted(kPlotMarginLeft, kPlotMarginTop,
                -kPlotMarginRight, -kPlotMarginBottom);
    }

    QPointF ToPixel(const QRect &plot_area, float x, float y) const
    {
        const float x_range = graph_->MaxX - graph_->MinX;
        const float y_range = graph_->MaxY - graph_->MinY;

        const float x_ratio =
                x_range == 0.0f ? 0.0f : (x - graph_->MinX) / x_range;
        const float y_ratio =
                y_range == 0.0f ? 0.0f : (y - graph_->MinY) / y_range;

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
        painter.drawText(plot_area.center().x() - 10, rect().bottom() - 12,
                "x");
        painter.save();
        painter.translate(18, plot_area.center().y());
        painter.rotate(-90);
        painter.drawText(0, 0, "y");
        painter.restore();
    }

    void DrawCurve(QPainter &painter, const QRect &plot_area,
            const Graph::Data &curve) const
    {
        const QColor color = ParseColor(curve.Color);
        const Graph::Segment &segment = curve.Value(parameter_);
        const int size = segment.Size();
        if (size == 0) return;

        if (curve.Point)
        {
            QPen pen(color, 1.5);
            painter.setPen(pen);
            painter.setBrush(color);

            constexpr double kPointRadius = 1.25;
            for (int i = 0; i < size; ++i)
            {
                const Graph::Point &pt = segment[i];
                const QPointF point = ToPixel(plot_area, pt.X(), pt.Y());
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
        for (int i = 0; i < size; ++i)
        {
            const Graph::Point &pt = segment[i];
            const QPointF point = ToPixel(plot_area, pt.X(), pt.Y());
            if (!started)
            {
                path.moveTo(point);
                started = true;
            } else
                path.lineTo(point);
        }
        painter.drawPath(path);
    }

    void DrawLegend(QPainter &painter, const QRect &plot_area) const
    {
        std::vector<LegendItem> items;
        for (const auto &curve : graph_->Segments)
        {
            if (curve == nullptr) continue;

            items.push_back(
                    { curve->Label, ParseColor(curve->Color),
                            curve->Point ?
                                    LegendSwatch::Point : LegendSwatch::Line });
        }

        ::DrawLegend(painter, plot_area, items);
    }

    const Graph *graph_;
    float parameter_ = 0.0f;
};

class PlotWindow final : public QWidget
{
public:
    explicit PlotWindow(const Graph *graph) : graph_(graph)
    {
        setWindowTitle(QString::fromStdString(graph->WindowTitle));
        resize(900, 700);

        auto *layout = new QVBoxLayout(this);

        title_label_ = new QLabel(this);
        layout->addWidget(title_label_);

        plot_canvas_ = new GraphCanvas(graph_, this);
        layout->addWidget(plot_canvas_, 1);

        slider_ = new QSlider(Qt::Horizontal, this);
        slider_->setRange(0, kSliderSteps);
        slider_->setValue(0);
        layout->addWidget(slider_);

        slider_->setVisible(graph_->Slider);
        if (graph_->Slider)
        {
            connect(slider_, &QSlider::valueChanged, this, [this](int value)
            {    UpdateDisplay(
                        SliderToParameter(value,
                                graph_->MinP, graph_->MaxP));
            });
        }

        UpdateDisplay(graph_->MinP);
    }

private:
    void UpdateDisplay(float parameter)
    {
        SetTitleLabel(*title_label_,
                graph_->Slider ?
                        graph_->Title(parameter) : graph_->WindowTitle);
        plot_canvas_->SetParameter(parameter);
    }

    const Graph *graph_;
    QLabel *title_label_ = nullptr;
    GraphCanvas *plot_canvas_ = nullptr;
    QSlider *slider_ = nullptr;
};

class GraphViewWindow final : public QWidget
{
public:
    GraphViewWindow(Graph *graph, float parameter) : graph_(graph)
    {
        setWindowTitle(QString::fromStdString(graph_->WindowTitle));
        resize(900, 700);

        auto *layout = new QVBoxLayout(this);

        title_label_ = new QLabel(this);
        SetTitleLabel(*title_label_,
                graph_->Slider ?
                        graph_->Title(parameter) : graph_->WindowTitle);
        layout->addWidget(title_label_);

        canvas_ = new GraphCanvas(graph_, this);
        layout->addWidget(canvas_, 1);
        canvas_->SetParameter(parameter);
    }

private:
    Graph *graph_;
    QLabel *title_label_ = nullptr;
    GraphCanvas *canvas_ = nullptr;
};

bool compare(Graph::Point &p1, Graph::Point &p2)
{
    return p1.X() > p2.X();
}

Graph::Point::Point(const float x_, const float y_) : x(x_), y(y_)
{
}

float Graph::Point::X() const
{
    return x;
}
;
float Graph::Point::Y() const
{
    return y;
}
;

Graph::Segment::Segment(const std::vector<Graph::Point> &points) : data(points)
{
    std::sort(data.begin(), data.end(), compare);
    min = data.begin()->X();
    max = data.rbegin()->X();
}

float Graph::Segment::Min() const
{
    return min;
}
float Graph::Segment::Max() const
{
    return max;
}
int Graph::Segment::Size() const
{
    return static_cast<int>(data.size());
}
const Graph::Point& Graph::Segment::operator[](size_t index) const
{
    return data.at(index);
}

Graph::Data::Data(float min_x, float max_x, std::string color,
        std::string label, bool point) : MinX(min_x), MaxX(max_x), Color(
        std::move(color)), Label(std::move(label)), Point(point)
{
}

Graph::Graph(const std::string &title, const bool slider, const float min_p,
        const float max_p, const float min_x, const float max_x,
        const float min_y, const float max_y,
        const std::vector<std::shared_ptr<Data>> &data) : WindowTitle(title), Slider(
        slider), MinP(min_p), MaxP(max_p), MinX(min_x), MaxX(max_x), MinY(
        min_y), MaxY(max_y), Segments(data)
{
}

void Graph::Plot()
{
    RunQT(std::make_unique < PlotWindow > (this));
}

void Graph::Show(const float parameter)
{
    RunQT(std::make_unique < GraphViewWindow > (this, parameter));
}
