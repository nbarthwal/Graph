#pragma once

#include <functional>
#include <memory>

class QWidget;

namespace plot_detail {

void RunQtApp(const std::function<std::unique_ptr<QWidget>()>& create_window);

}  // namespace plot_detail
