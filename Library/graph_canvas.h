#pragma once

#include "graph.h"

#include <QPointF>
#include <QRect>
#include <QWidget>

class QPainter;
class QPaintEvent;

class GraphCanvas final : public QWidget
{
public:
    explicit GraphCanvas(Graph& graph, QWidget* parent = nullptr);

    void SetParameter(float parameter);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QRect PlotArea() const;
    QPointF ToPixel(const QRect& plot_area, float x, float y) const;
    void DrawBackground(QPainter& painter) const;
    void DrawGrid(QPainter& painter, const QRect& plot_area) const;
    void DrawAxes(QPainter& painter, const QRect& plot_area) const;
    void DrawCurve(QPainter& painter, const QRect& plot_area, const Curve& curve) const;

    Graph& graph_;
    float parameter_ = 0.0f;
};
