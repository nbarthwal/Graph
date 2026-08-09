#include "histogram_plot.h"

#include "histogram_canvas.h"
#include "plot_app.h"
#include "plot_detail.h"

#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

namespace {

using plot_detail::kSliderSteps;
using plot_detail::SliderToParameter;

class HistogramWindow final : public QWidget
{
public:
    explicit HistogramWindow(Histogram& histogram)
        : histogram_(histogram)
    {
        setWindowTitle("Histogram Plot");
        resize(900, 700);

        auto* layout = new QVBoxLayout(this);

        parameter_label_ = new QLabel(this);
        layout->addWidget(parameter_label_);

        histogram_canvas_ = new HistogramCanvas(histogram_, this);
        layout->addWidget(histogram_canvas_, 1);

        slider_ = new QSlider(Qt::Horizontal, this);
        slider_->setRange(0, kSliderSteps);
        slider_->setValue(0);
        layout->addWidget(slider_);

        connect(slider_, &QSlider::valueChanged, this, [this](int value) {
            const float parameter =
                SliderToParameter(value, histogram_.MinP(), histogram_.MaxP());
            parameter_label_->setText(
                QString("Parameter p = %1").arg(static_cast<double>(parameter), 0, 'g', 4));
            histogram_canvas_->SetParameter(parameter);
        });

        const float initial_parameter = histogram_.MinP();
        parameter_label_->setText(
            QString("Parameter p = %1").arg(static_cast<double>(initial_parameter), 0, 'g', 4));
        histogram_canvas_->SetParameter(initial_parameter);
    }

private:
    Histogram& histogram_;
    QLabel* parameter_label_ = nullptr;
    HistogramCanvas* histogram_canvas_ = nullptr;
    QSlider* slider_ = nullptr;
};

}  // namespace

void PlotHistogram(Histogram& histogram)
{
    plot_detail::RunQtApp([&histogram]() { return std::make_unique<HistogramWindow>(histogram); });
}
