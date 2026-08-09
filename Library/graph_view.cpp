#include "graph.h"
#include "graph_impl.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

namespace
{

    using plot_detail::SetTitleLabel;

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

void Graph::Show(const float parameter)
{
    plot_detail::RunQtApp([this, parameter]()
    {
        return std::make_unique<GraphViewWindow>(*this, parameter);
    });
}
