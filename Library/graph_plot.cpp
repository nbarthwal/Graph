#include "graph.h"
#include "graph_impl.h"

#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

namespace
{

    using plot_detail::kSliderSteps;
    using plot_detail::SetTitleLabel;
    using plot_detail::SliderToParameter;

    class PlotWindow final : public QWidget
    {
    public:
        explicit PlotWindow(const Graph &graph) :
                graph_(graph)
        {
            setWindowTitle(QString::fromStdString(graph.Title()));
            resize(900, 700);

            auto *layout = new QVBoxLayout(this);

            title_label_ = new QLabel(this);
            layout->addWidget(title_label_);

            plot_canvas_ = new GraphCanvas(graph_, this);
            layout->addWidget(plot_canvas_, 1);

            parameter_label_ = new QLabel(this);
            layout->addWidget(parameter_label_);

            slider_ = new QSlider(Qt::Horizontal, this);
            slider_->setRange(0, kSliderSteps);
            slider_->setValue(0);
            layout->addWidget(slider_);

            connect(slider_, &QSlider::valueChanged, this,
                    [this](
                            int value)
                            {
                                UpdateDisplay(
                                        SliderToParameter(value, graph_.MinP(),
                                                graph_.MaxP()));
                            });

            UpdateDisplay(graph_.MinP());
        }

    private:
        void UpdateDisplay(float parameter)
        {
            SetTitleLabel(*title_label_, graph_.Title(parameter));
            parameter_label_->setText(
                    QString("Parameter p = %1").arg(
                            static_cast<double>(parameter), 0, 'g', 4));
            plot_canvas_->SetParameter(parameter);
        }

        const Graph &graph_;
        QLabel *title_label_ = nullptr;
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
