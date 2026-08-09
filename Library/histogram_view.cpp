#include "graph.h"
#include "graph_impl.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

namespace
{

    using plot_detail::SetTitleLabel;

    class HistogramViewWindow final : public QWidget
    {
    public:
        HistogramViewWindow(Histogram &histogram, float parameter) :
                histogram_(histogram)
        {
            setWindowTitle("Histogram View");
            resize(900, 700);

            auto *layout = new QVBoxLayout(this);

            title_label_ = new QLabel(this);
            SetTitleLabel(*title_label_, histogram_.Title(parameter));
            layout->addWidget(title_label_);

            canvas_ = new HistogramCanvas(histogram_, this);
            layout->addWidget(canvas_, 1);
            canvas_->SetParameter(parameter);
        }

    private:
        Histogram &histogram_;
        QLabel *title_label_ = nullptr;
        HistogramCanvas *canvas_ = nullptr;
    };

}  // namespace

void Histogram::Show(const float parameter)
{
    plot_detail::RunQtApp([this, parameter]()
    {
        return std::make_unique<HistogramViewWindow>(*this, parameter);
    });
}
