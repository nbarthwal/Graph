#include "graph.h"
#include "common.h"

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
#include <QColor>
#include <QPointF>
#include <QRect>


class GraphCanvas final : public QWidget
{
public:
    explicit GraphCanvas(const Graph::SliderPlot *g, QWidget *parent = nullptr):
        QWidget(parent), graph(g)
    {
        setMinimumSize(640, 480);
        setAutoFillBackground(true);
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(252, 252, 252));
        setPalette(palette);
    }

    void SetParameter(float param)
    {
        parameter = param;
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

        for (const auto& curve : graph->DataSet())
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
        const float x_range = graph->MaxX - graph->MinX;
        const float y_range = graph->MaxY - graph->MinY;

        const float x_ratio =
                x_range == 0.0f ? 0.0f : (x - graph->MinX) / x_range;
        const float y_ratio =
                y_range == 0.0f ? 0.0f : (y - graph->MinY) / y_range;

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
            const Graph::SliderPlot::Data &curve) const
    {
        const QColor color = ParseColor(curve.Color);
        const unique_ptr<Graph::Segment>& segment = curve.Value(parameter);
        const int size = segment->Size();
        if (size == 0) return;

        if (curve.Point)
        {
            QPen pen(color, 1.5);
            painter.setPen(pen);
            painter.setBrush(color);

            constexpr double kPointRadius = 1.25;
            for (int i = 0; i < size; ++i)
            {
                const QPointF point = ToPixel(plot_area,
                                              segment->X(i), segment->Y(i));
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
            const QPointF point = ToPixel(plot_area, segment->X(i), segment->Y(i));
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
        set<pair<string, string>> labels;
        vector<LegendItem> items;
        for (const auto& curve : graph->DataSet())
        {
            pair<string, string> p = make_pair(curve->Label, curve->Color);
            if (labels.count(p) > 0)
                continue;

            labels.emplace(p);
            items.push_back(
                { curve->Label, ParseColor(curve->Color),
                    curve->Point ? LegendSwatch::Point : LegendSwatch::Line });
        }

        ::DrawLegend(painter, plot_area, items);
    }

    const Graph::SliderPlot *graph;
    float parameter = 0.0f;
};


class PlotWindow final : public QWidget
{
public:
    explicit PlotWindow(const Graph::SliderPlot *g) : graph(g)
    {
        setWindowTitle(QString::fromStdString(graph->WindowTitle));
        resize(900, 700);

        auto *layout = new QVBoxLayout(this);

        title_label = new QLabel(this);
        layout->addWidget(title_label);

        plot_canvas = new GraphCanvas(graph, this);
        layout->addWidget(plot_canvas, 1);

        slider_ = new QSlider(Qt::Horizontal, this);
        slider_->setRange(0, kSliderSteps);
        slider_->setValue(0);
        layout->addWidget(slider_);

        slider_->setVisible(graph->Slider);
        if (graph->Slider)
            connect(slider_, &QSlider::valueChanged, this, [this](int value)
            { UpdateDisplay(
                SliderToParameter(value, graph->MinP, graph->MaxP)); });


        UpdateDisplay(graph->MinP);
    }

private:
    void UpdateDisplay(float parameter)
    {
        SetTitleLabel(*title_label,
                graph->Slider ?
                        graph->Title(parameter) : graph->WindowTitle);
        plot_canvas->SetParameter(parameter);
    }

    const Graph::SliderPlot *graph;
    QLabel *title_label = nullptr;
    GraphCanvas *plot_canvas = nullptr;
    QSlider *slider_ = nullptr;
};


class GraphViewWindow final : public QWidget
{
public:
    GraphViewWindow(Graph::SliderPlot *g, float parameter) : graph(g)
    {
        setWindowTitle(QString::fromStdString(graph->WindowTitle));
        resize(900, 700);

        auto *layout = new QVBoxLayout(this);

        title_label = new QLabel(this);
        SetTitleLabel(*title_label,
                graph->Slider ?
                        graph->Title(parameter) : graph->WindowTitle);
        layout->addWidget(title_label);

        canvas = new GraphCanvas(graph, this);
        layout->addWidget(canvas, 1);
        canvas->SetParameter(parameter);
    }

private:
    Graph::SliderPlot *graph;
    QLabel *title_label = nullptr;
    GraphCanvas *canvas = nullptr;
};


/*
Graph::Point::Point(const float x, const float y): X(x), Y(y) { }


bool compare(unique_ptr<Graph::Point>& p1, unique_ptr<Graph::Point>&
 p2)
    { return p1->X > p2->X; }

Graph::Segment::Segment(const std::vector<Graph::Point>& points): data(points.size())
{
    int size = (int) points.size();
    for(int i=0; i < size; ++i)
        data[i] = make_unique<Graph::Point>(points[i].X, points[i].Y);

    std::sort(data.begin(), data.end(), compare);
    min = data[0]->X;
    max = data[size-1]->X;
}

float Graph::Segment::Min() const
    { return min; }

float Graph::Segment::Max() const
    { return max; }

int Graph::Segment::Size() const
    { return static_cast<int>(data.size()); }

void Graph::Segment::Set(const int index, const float y)
    { data[index] = make_unique<Graph::Point>(data[index]->X, y); }

const float Graph::Segment::X(size_t index) const
    { return data.at(index)->X; }

const float Graph::Segment::Y(size_t index) const
    { return data.at(index)->Y; }

Graph::Data::Data(const float min_x, const float max_x, const string& color,
                  const string& label, bool point):
    MinX(min_x), MaxX(max_x), Color(std::move(color)), Label(std::move(label)),
    Point(point) { }

Graph::Graph(const string& title, const bool slider, const float min_p,
             const float max_p, const float min_x, const float max_x,
             const float min_y, const float max_y):
                 WindowTitle(title), Slider(slider), MinP(min_p), MaxP(max_p),
                 MinX(min_x), MaxX(max_x), MinY(min_y), MaxY(max_y) { }

void Graph::Plot()
    { RunQT(std::make_unique<PlotWindow>(this)); }

void Graph::Show(const float parameter)
    { RunQT(std::make_unique <GraphViewWindow>(this, parameter)); }
*/