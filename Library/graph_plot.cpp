#include "graph.h"
#include "graph_impl.h"

#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

namespace
{

    using plot_detail::kSliderSteps;
    using plot_detail::SliderToParameter;

    class PlotWindow final : public QWidget
    {
    public:
        explicit PlotWindow(const Graph &graph) :
                graph_(graph)
        {
            setWindowTitle("Graph Plot");
            resize(900, 700);

            auto *layout = new QVBoxLayout(this);

            parameter_label_ = new QLabel(this);
            layout->addWidget(parameter_label_);

            plot_canvas_ = new GraphCanvas(graph_, this);
            layout->addWidget(plot_canvas_, 1);

            slider_ = new QSlider(Qt::Horizontal, this);
            slider_->setRange(0, kSliderSteps);
            slider_->setValue(0);
            layout->addWidget(slider_);

            connect(slider_, &QSlider::valueChanged, this,
                    [this](
                            int value)
                            {
                                const float parameter =
                                SliderToParameter(value, graph_.MinP(), graph_.MaxP());
                                parameter_label_->setText(
                                        QString("Parameter p = %1").arg(static_cast<double>(parameter), 0, 'g', 4));
                                plot_canvas_->SetParameter(parameter);
                            });

            const float initial_parameter = graph_.MinP();
            parameter_label_->setText(
                    QString("Parameter p = %1").arg(
                            static_cast<double>(initial_parameter), 0, 'g', 4));
            plot_canvas_->SetParameter(initial_parameter);
        }

    private:
        const Graph &graph_;
        QLabel *parameter_label_ = nullptr;
        GraphCanvas *plot_canvas_ = nullptr;
        QSlider *slider_ = nullptr;
    };

}  // namespace

void Plot(const Graph& graph)
{
    plot_detail::RunQtApp([&graph]()
    {   return std::make_unique<PlotWindow>(graph);});
}
