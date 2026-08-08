#pragma once

#include "histogram.h"

#include <QRect>
#include <QWidget>

class QPainter;
class QPaintEvent;

class HistogramCanvas final : public QWidget
{
public:
    explicit HistogramCanvas(Histogram& histogram, QWidget* parent = nullptr);

    void SetParameter(float parameter);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QRect PlotArea() const;
    float ToPixelX(const QRect& plot_area, float x) const;
    float ToPixelY(const QRect& plot_area, float y) const;
    void DrawBackground(QPainter& painter) const;
    void DrawGrid(QPainter& painter, const QRect& plot_area) const;
    void DrawAxes(QPainter& painter, const QRect& plot_area) const;
    void DrawHistograms(QPainter& painter, const QRect& plot_area) const;

    Histogram& histogram_;
    float parameter_ = 0.0f;
};
