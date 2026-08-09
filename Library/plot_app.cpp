#include "plot_app.h"

#include <QApplication>
#include <QWidget>

#include <memory>

namespace plot_detail {

void RunQtApp(const std::function<std::unique_ptr<QWidget>()>& create_window)
{
    const bool owns_application = QApplication::instance() == nullptr;
    std::unique_ptr<QApplication> owned_application;
    int argc = 0;

    if (owns_application) {
        owned_application = std::make_unique<QApplication>(argc, nullptr);
    }

    std::unique_ptr<QWidget> window = create_window();
    window->show();

    if (owns_application) {
        QApplication::exec();
    }
}

}  // namespace plot_detail
