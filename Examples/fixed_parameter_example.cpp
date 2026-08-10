#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>

constexpr float kFixedParameter = 1.0f;
constexpr float kTwoPi = 6.28318f;

class FixedSineCurve final : public Graph::Data
    {
    public:
        FixedSineCurve(float frequency, std::string color, bool point) :
                frequency_(frequency), color_(std::move(color)), point_(point)
        {
        }

        float MaxX() const override
        {
            return kTwoPi;
        }
        float MinX() const override
        {
            return 0.0f;
        }
        std::string Color() const override
        {
            return color_;
        }
        std::string Label() const override
        {
            if (frequency_ == 1.0f)
            {
                return "sin(x)";
            }
            return "sin(" + std::to_string(frequency_) + "x)";
        }
        float Value(float p, float x) const override
        {
            return p * std::sin(frequency_ * x);
        }
        bool Point() const override
        {
            return point_;
        }

    private:
        float frequency_;
        std::string color_;
        bool point_;
    };

    class FixedParameterGraph final : public Graph
    {
    public:
        FixedParameterGraph()
        {
            curves_.push_back(
                    std::make_unique<FixedSineCurve>(1.0f, "blue", false));
            curves_.push_back(
                    std::make_unique<FixedSineCurve>(2.0f, "red", true));
        }

        float MaxX() const override
        {
            return kTwoPi;
        }
        float MinX() const override
        {
            return 0.0f;
        }
        float MaxY() const override
        {
            return 1.5f;
        }
        float MinY() const override
        {
            return -1.5f;
        }
        float MaxP() const override
        {
            return kFixedParameter;
        }
        float MinP() const override
        {
            return kFixedParameter;
        }

        std::vector<const Graph::Data*> Curves() const override
        {
            std::vector<const Graph::Data*> curves;
            curves.reserve(curves_.size());
            for (const auto &curve : curves_)
            {
                curves.push_back(curve.get());
            }
            return curves;
        }

        std::string Title() const override
        {
            return "Fixed Parameter Graph";
        }

        std::string Title(const float parameter) const override
        {
            return "Fixed Parameter Graph (p = " + std::to_string(parameter)
                    + ")";
        }

    private:
        std::vector<std::unique_ptr<Graph::Data>> curves_;
    };

    class FixedHistogramData final : public Histogram::Data
    {
    public:
        FixedHistogramData(std::vector<float> centers, float bin_width,
                std::vector<float> counts, std::string color,
                std::string label) :
                centers_(std::move(centers)), bin_width_(bin_width),
                counts_(std::move(counts)), color_(std::move(color)),
                label_(std::move(label))
        {
        }

        std::string Color() const override
        {
            return color_;
        }
        std::string Label() const override
        {
            return label_;
        }
        std::size_t BinCount() const override
        {
            return centers_.size();
        }
        float BinCenter(std::size_t bin) const override
        {
            return centers_.at(bin);
        }
        float BinWidth() const override
        {
            return bin_width_;
        }
        float Count(float /*p*/, std::size_t bin) const override
        {
            return counts_.at(bin);
        }

    private:
        std::vector<float> centers_;
        float bin_width_;
        std::vector<float> counts_;
        std::string color_;
        std::string label_;
    };

    class FixedParameterHistogram final : public Histogram
    {
    public:
        FixedParameterHistogram()
        {
            data_sets_.push_back(std::make_unique<FixedHistogramData>(
                    std::vector<float> { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f }, 1.0f,
                    std::vector<float> { 3.0f, 6.0f, 4.0f, 2.0f, 1.0f },
                    "blue", "Counts"));
        }

        float MaxX() const override
        {
            return 6.0f;
        }
        float MinX() const override
        {
            return 0.0f;
        }
        float MaxY() const override
        {
            return 8.0f;
        }
        float MinY() const override
        {
            return 0.0f;
        }
        float MaxP() const override
        {
            return kFixedParameter;
        }
        float MinP() const override
        {
            return kFixedParameter;
        }

        std::vector<const Histogram::Data*> DataSets() const override
        {
            std::vector<const Histogram::Data*> data_sets;
            data_sets.reserve(data_sets_.size());
            for (const auto &data_set : data_sets_)
            {
                data_sets.push_back(data_set.get());
            }
            return data_sets;
        }

        std::string Title() const override
        {
            return "Fixed Parameter Histogram";
        }

        std::string Title(const float parameter) const override
        {
            return "Fixed Parameter Histogram (p = "
                    + std::to_string(parameter) + ")";
        }

    private:
        std::vector<std::unique_ptr<Histogram::Data>> data_sets_;
};

int main(int argc, char *argv[])
{
    if (argc > 1 && std::string(argv[1]) == "histogram")
    {
        FixedParameterHistogram histogram;
        Plot(histogram);
    }
    else
    {
        FixedParameterGraph graph;
        graph.Plot();
    }
    return 0;
}
