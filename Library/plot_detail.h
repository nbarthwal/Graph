#pragma once

#include <QColor>
#include <string>
#include <unordered_map>

namespace plot_detail {

constexpr int kSliderSteps = 1000;
constexpr int kPlotMarginLeft = 60;
constexpr int kPlotMarginRight = 20;
constexpr int kPlotMarginTop = 20;
constexpr int kPlotMarginBottom = 50;

inline float SliderToParameter(int slider_value, float min_p, float max_p)
{
    const float t = static_cast<float>(slider_value) / static_cast<float>(kSliderSteps);
    return min_p + t * (max_p - min_p);
}

inline QColor ParseColor(const std::string& color)
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

}  // namespace plot_detail
