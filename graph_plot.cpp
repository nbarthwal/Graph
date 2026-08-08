#include "graph_plot.h"

#include <matplot/matplot.h>

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include <cmath>
#include <memory>
#include <string>
#include <vector>

namespace {

constexpr int kSliderSteps = 1000;
constexpr std::size_t kSampleCount = 500;

float SliderToParameter(int slider_value, float min_p, float max_p)
{
    const float t = static_cast<float>(slider_value) / static_cast<float>(kSliderSteps);
    return min_p + t * (max_p - min_p);
}

std::vector<double> SampleX(double min_x, double max_x)
{
  return matplot::linspace(min_x, max_x, kSampleCount);
}

std::vector<double> SampleY(const Curve& curve, float parameter, const std::vector<double>& x_values)
{
    std::vector<double> y_values;
    y_values.reserve(x_values.size());

    for (const double x : x_values) {
        y_values.push_back(static_cast<double>(curve.Value(parameter, static_cast<float>(x))));
    }

    return y_values;
}

class PlotWindow final : public QWidget
{
public:
    explicit PlotWindow(Graph& graph)
        : graph_(graph)
    {
        setWindowTitle("Graph Plot");
        resize(900, 700);

        auto* layout = new QVBoxLayout(this);

        parameter_label_ = new QLabel(this);
        layout->addWidget(parameter_label_);

        slider_ = new QSlider(Qt::Horizontal, this);
        slider_->setRange(0, kSliderSteps);
        slider_->setValue(0);
        layout->addWidget(slider_);

        figure_ = matplot::figure(true);
        axes_ = figure_->current_axes();
        axes_->xlim({graph_.MinX(), graph_.MaxX()});
        axes_->ylim({graph_.MinY(), graph_.MaxY()});
        axes_->xlabel("x");
        axes_->ylabel("y");
        figure_->draw();

        connect(slider_, &QSlider::valueChanged, this, [this](int value) {
            UpdatePlot(SliderToParameter(value, graph_.MinP(), graph_.MaxP()));
        });

        UpdatePlot(graph_.MinP());
    }

private:
    void UpdatePlot(float parameter)
    {
        parameter_label_->setText(
            QString("Parameter p = %1").arg(static_cast<double>(parameter), 0, 'g', 4));

        axes_->clear();
        axes_->hold(false);
        axes_->xlim({graph_.MinX(), graph_.MaxX()});
        axes_->ylim({graph_.MinY(), graph_.MaxY()});
        axes_->xlabel("x");
        axes_->ylabel("y");

        for (const Curve* curve : graph_.Curves()) {
            if (curve == nullptr) {
                continue;
            }

            const std::vector<double> x_values =
                SampleX(curve->MinX(), curve->MaxX());
            const std::vector<double> y_values = SampleY(*curve, parameter, x_values);

            if (curve->Point()) {
                matplot::scatter(axes_, x_values, y_values)
                    ->color(curve->Color())
                    .marker_size(8);
            } else {
                matplot::plot(axes_, x_values, y_values)->color(curve->Color()).line_width(2);
            }
        }

        figure_->draw();
    }

    Graph& graph_;
    QLabel* parameter_label_ = nullptr;
    QSlider* slider_ = nullptr;
    matplot::figure_handle figure_;
    matplot::axes_handle axes_;
};

}  // namespace

void Plot(Graph& graph)
{
    int argc = 0;
    char* argv[] = {nullptr};
    QApplication app(argc, argv);

    PlotWindow window(graph);
    window.show();

    app.exec();
}
