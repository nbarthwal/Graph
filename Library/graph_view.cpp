#include "graph_view.h"

#include "graph_canvas.h"
#include "plot_app.h"

#include <QVBoxLayout>
#include <QWidget>

namespace {

class GraphViewWindow final : public QWidget
{
public:
    GraphViewWindow(Graph& graph, float parameter)
        : graph_(graph)
    {
        setWindowTitle("Graph View");
        resize(900, 700);

        auto* layout = new QVBoxLayout(this);
        canvas_ = new GraphCanvas(graph_, this);
        layout->addWidget(canvas_, 1);
        canvas_->SetParameter(parameter);
    }

private:
    Graph& graph_;
    GraphCanvas* canvas_ = nullptr;
};

}  // namespace

void ShowGraph(Graph& graph, float parameter)
{
    plot_detail::RunQtApp([&graph, parameter]() {
        return std::make_unique<GraphViewWindow>(graph, parameter);
    });
}
