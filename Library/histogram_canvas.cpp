#include "graph_impl.h"

#include <QPainter>
#include <QPen>

#include <algorithm>
#include <vector>

using plot_detail::kPlotMarginBottom;
using plot_detail::kPlotMarginLeft;
using plot_detail::kPlotMarginRight;
using plot_detail::kPlotMarginTop;
using plot_detail::ParseColor;

HistogramCanvas::HistogramCanvas(Histogram &histogram, QWidget *parent) :
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
    const std::vector<const HistogramData*> data_sets = histogram_.DataSets();
    if (data_sets.empty())
    {
        return;
    }

    const std::size_t data_set_count = data_sets.size();
    const float baseline_y = ToPixelY(plot_area, histogram_.MinY());

    for (std::size_t data_index = 0; data_index < data_set_count; ++data_index)
    {
        const HistogramData *data = data_sets[data_index];
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
