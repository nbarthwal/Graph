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

    class HistogramWindow final : public QWidget
    {
    public:
        explicit HistogramWindow(const Histogram &histogram) :
                histogram_(histogram)
        {
            setWindowTitle(QString::fromStdString(histogram.Title()));
            resize(900, 700);

            auto *layout = new QVBoxLayout(this);

            title_label_ = new QLabel(this);
            layout->addWidget(title_label_);

            histogram_canvas_ = new HistogramCanvas(histogram_, this);
            layout->addWidget(histogram_canvas_, 1);

            slider_ = new QSlider(Qt::Horizontal, this);
            slider_->setRange(0, kSliderSteps);
            slider_->setValue(0);
            layout->addWidget(slider_);

            connect(slider_, &QSlider::valueChanged, this,
                    [this](
                            int value)
                            {
                                UpdateDisplay(
                                        SliderToParameter(value, histogram_.MinP(),
                                                histogram_.MaxP()));
                            });

            UpdateDisplay(histogram_.MinP());
        }

    private:
        void UpdateDisplay(float parameter)
        {
            SetTitleLabel(*title_label_, histogram_.Title(parameter));
            histogram_canvas_->SetParameter(parameter);
        }

        const Histogram &histogram_;
        QLabel *title_label_ = nullptr;
        HistogramCanvas *histogram_canvas_ = nullptr;
        QSlider *slider_ = nullptr;
    };

}  // namespace

void Plot(const Histogram& histogram)
{
    plot_detail::RunQtApp([&histogram]()
    {   return std::make_unique<HistogramWindow>(histogram);});
}
