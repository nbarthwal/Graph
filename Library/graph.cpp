#include "graph.h"

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

            slider_ = new QSlider(Qt::Horizontal, this);
            slider_->setRange(0, kSliderSteps);
            slider_->setValue(0);
            layout->addWidget(slider_);

            const bool show_slider = graph_.MaxP() != graph_.MinP();
            slider_->setVisible(show_slider);

            if (show_slider)
            {
                connect(slider_, &QSlider::valueChanged, this,
                        [this](
                                int value)
                                {
                                    UpdateDisplay(
                                            SliderToParameter(value, graph_.MinP(),
                                                    graph_.MaxP()));
                                });
            }

            UpdateDisplay(graph_.MinP());
        }

    private:
        void UpdateDisplay(float parameter)
        {
            SetTitleLabel(*title_label_, graph_.Title(parameter));
            plot_canvas_->SetParameter(parameter);
        }

        const Graph &graph_;
        QLabel *title_label_ = nullptr;
        GraphCanvas *plot_canvas_ = nullptr;
        QSlider *slider_ = nullptr;
    };

    class GraphViewWindow final : public QWidget
    {
    public:
        GraphViewWindow(Graph &graph, float parameter) :
                graph_(graph)
        {
            setWindowTitle(QString::fromStdString(graph_.Title()));
            resize(900, 700);

            auto *layout = new QVBoxLayout(this);

            title_label_ = new QLabel(this);
            SetTitleLabel(*title_label_, graph_.Title(parameter));
            layout->addWidget(title_label_);

            canvas_ = new GraphCanvas(graph_, this);
            layout->addWidget(canvas_, 1);
            canvas_->SetParameter(parameter);
        }

    private:
        Graph &graph_;
        QLabel *title_label_ = nullptr;
        GraphCanvas *canvas_ = nullptr;
    };

}  // namespace

void Plot(const Graph& graph)
{
    plot_detail::RunQtApp([&graph]()
    {   return std::make_unique<PlotWindow>(graph);});
}

void Graph::Show(const float parameter)
{
    plot_detail::RunQtApp([this, parameter]()
    {
        return std::make_unique<GraphViewWindow>(*this, parameter);
    });
}
