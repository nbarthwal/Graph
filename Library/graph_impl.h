#pragma once

class Graph;
class Histogram;

#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QRect>
#include <QWidget>

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class QPainter;
class QPaintEvent;

namespace plot_detail
{

    constexpr int kSliderSteps = 1000;
    constexpr int kPlotMarginLeft = 60;
    constexpr int kPlotMarginRight = 20;
    constexpr int kPlotMarginTop = 20;
    constexpr int kPlotMarginBottom = 50;

    inline float SliderToParameter(int slider_value, float min_p, float max_p)
    {
        const float t = static_cast<float>(slider_value)
                / static_cast<float>(kSliderSteps);
        return min_p + t * (max_p - min_p);
    }

    inline QColor ParseColor(const std::string &color)
    {
        static const std::unordered_map<std::string, QColor> named_colors = { {
                "red", QColor(220, 50, 47) }, { "r", QColor(220, 50, 47) }, {
                "blue", QColor(38, 139, 210) }, { "b", QColor(38, 139, 210) }, {
                "green", QColor(133, 153, 0) }, { "g", QColor(133, 153, 0) }, {
                "orange", QColor(203, 75, 22) }, { "purple", QColor(108, 113,
                196) }, { "black", QColor(0, 0, 0) }, { "k", QColor(0, 0, 0) },
                { "white", QColor(255, 255, 255) },
                { "w", QColor(255, 255, 255) },
                { "cyan", QColor(42, 161, 152) }, { "magenta", QColor(211, 54,
                        130) }, { "yellow", QColor(181, 137, 0) }, };

        const auto it = named_colors.find(color);
        if (it != named_colors.end())
        {
            return it->second;
        }

        const QColor parsed(QString::fromStdString(color));
        return parsed.isValid() ? parsed : QColor(38, 139, 210);
    }

    inline void SetTitleLabel(QLabel &label, const std::string &title)
    {
        QFont title_font = label.font();
        title_font.setPointSize(20);
        title_font.setBold(true);
        label.setFont(title_font);
        label.setAlignment(Qt::AlignCenter);
        label.setText(QString::fromStdString(title));
    }

    enum class LegendSwatch
    {
        Line,
        Point,
        Bar,
    };

    struct LegendItem
    {
        std::string label;
        QColor color;
        LegendSwatch swatch = LegendSwatch::Line;
    };

    inline void DrawLegend(QPainter &painter, const QRect &plot_area,
            const std::vector<LegendItem> &items)
    {
        if (items.empty())
        {
            return;
        }

        constexpr int kPadding = 8;
        constexpr int kSwatchWidth = 20;
        constexpr int kSwatchHeight = 12;
        constexpr int kRowSpacing = 6;
        constexpr int kTextOffset = 8;
        constexpr int kLegendInset = 8;

        QFont font = painter.font();
        font.setPointSize(10);
        painter.setFont(font);
        const QFontMetrics metrics(font);

        int max_text_width = 0;
        for (const LegendItem &item : items)
        {
            max_text_width = std::max(max_text_width,
                    metrics.horizontalAdvance(
                            QString::fromStdString(item.label)));
        }

        const int legend_width =
                kPadding * 2 + kSwatchWidth + kTextOffset + max_text_width;
        const int row_height = std::max(kSwatchHeight, metrics.height());
        const int legend_height = kPadding * 2
                + static_cast<int>(items.size()) * row_height
                + static_cast<int>(items.size() - 1) * kRowSpacing;

        const QRect legend_rect(plot_area.right() - legend_width - kLegendInset,
                plot_area.top() + kLegendInset, legend_width, legend_height);

        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRect(legend_rect);
        painter.setPen(QPen(Qt::black, 1.0));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(legend_rect);

        int y = legend_rect.top() + kPadding;
        for (const LegendItem &item : items)
        {
            const int swatch_y = y + (row_height - kSwatchHeight) / 2;
            const QRect swatch_rect(legend_rect.left() + kPadding, swatch_y,
                    kSwatchWidth, kSwatchHeight);

            if (item.swatch == LegendSwatch::Point)
            {
                painter.setPen(QPen(item.color, 1.5));
                painter.setBrush(item.color);
                painter.drawEllipse(swatch_rect.center(), 4, 4);
            }
            else if (item.swatch == LegendSwatch::Bar)
            {
                painter.setPen(QPen(item.color.darker(120), 1.0));
                painter.setBrush(item.color);
                painter.drawRect(swatch_rect);
            }
            else
            {
                painter.setPen(QPen(item.color, 2.0));
                painter.drawLine(swatch_rect.left(), swatch_rect.center().y(),
                        swatch_rect.right(), swatch_rect.center().y());
            }

            painter.setPen(Qt::black);
            painter.drawText(
                    legend_rect.left() + kPadding + kSwatchWidth + kTextOffset,
                    y + metrics.ascent(), QString::fromStdString(item.label));
            y += row_height + kRowSpacing;
        }
        painter.restore();
    }

    void RunQtApp(
            const std::function<std::unique_ptr<QWidget>()> &create_window);

}  // namespace plot_detail

class HistogramCanvas final : public QWidget
{
public:
    explicit HistogramCanvas(const Histogram &histogram, QWidget *parent = nullptr);

    void SetParameter(float parameter);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QRect PlotArea() const;
    float ToPixelX(const QRect &plot_area, float x) const;
    float ToPixelY(const QRect &plot_area, float y) const;
    void DrawBackground(QPainter &painter) const;
    void DrawGrid(QPainter &painter, const QRect &plot_area) const;
    void DrawAxes(QPainter &painter, const QRect &plot_area) const;
    void DrawHistograms(QPainter &painter, const QRect &plot_area) const;
    void DrawLegend(QPainter &painter, const QRect &plot_area) const;

    const Histogram &histogram_;
    float parameter_ = 0.0f;
};
