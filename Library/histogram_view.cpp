#include "histogram_view.h"

#include "histogram_canvas.h"
#include "plot_app.h"

#include <QVBoxLayout>
#include <QWidget>

namespace {

class HistogramViewWindow final : public QWidget
{
public:
    HistogramViewWindow(Histogram& histogram, float parameter)
        : histogram_(histogram)
    {
        setWindowTitle("Histogram View");
        resize(900, 700);

        auto* layout = new QVBoxLayout(this);
        canvas_ = new HistogramCanvas(histogram_, this);
        layout->addWidget(canvas_, 1);
        canvas_->SetParameter(parameter);
    }

private:
    Histogram& histogram_;
    HistogramCanvas* canvas_ = nullptr;
};

}  // namespace

void ShowHistogram(Histogram& histogram, float parameter)
{
    plot_detail::RunQtApp([&histogram, parameter]() {
        return std::make_unique<HistogramViewWindow>(histogram, parameter);
    });
}
