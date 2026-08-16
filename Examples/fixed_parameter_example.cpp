#include <cmath>
#include <graph.h>
#include <histogram.h>
#include <memory>
#include <string>
#include <vector>

constexpr float kFixedParameter = 1.0f;
constexpr float kTwoPi = 6.28318f;

class FixedParameterGraph final : public Graph::DynamicPlot
{
public:
    FixedParameterGraph() : Graph::DynamicPlot("Fixed Parameter Graph",
            Graph::Canvas(0.0f, kTwoPi, -1.5f, 1.5f), kFixedParameter,
            kFixedParameter)
    {
    }

    std::string Title(float parameter) const override
    {
        return "Fixed Parameter Graph (p = " + std::to_string(parameter) + ")";
    }

    Graph::DynamicData Eval(float p) const override
    {
        curves_.clear();
        result_.clear();
        result_.reserve(2);

        curves_.push_back(MakeCurve(1.0f, "blue", false, p));
        result_.push_back(curves_.back().get());

        curves_.push_back(MakeCurve(2.0f, "red", true, p));
        result_.push_back(curves_.back().get());

        return result_;
    }

private:
    struct Curve final : public Graph::Data
    {
        Curve(std::string color, std::string label, Graph::points pts,
                bool bullet) : Graph::Data(color, label, pts, bullet)
        {
        }
    };

    static std::unique_ptr<Curve> MakeCurve(float frequency,
            const std::string& color, bool bullet, float p)
    {
        constexpr int kPointCount = 500;
        Graph::points pts;
        for (int i = 0; i < kPointCount; ++i)
        {
            const float x = kTwoPi * static_cast<float>(i)
                    / static_cast<float>(kPointCount - 1);
            pts[x] = p * std::sin(frequency * x);
        }
        const std::string label = frequency == 1.0f ?
                "sin(x)" : "sin(" + std::to_string(frequency) + "x)";
        return std::make_unique<Curve>(color, label, std::move(pts), bullet);
    }

    mutable std::vector<std::unique_ptr<Curve>> curves_;
    mutable std::vector<Graph::Data*> result_;
};

class FixedHistogramData final : public Histogram::Data
{
public:
    FixedHistogramData(std::vector<float> counts, std::string color,
            std::string label) : Histogram::Data(std::move(color),
            std::move(label), static_cast<int>(counts.size())), counts_(
            std::move(counts))
    {
    }

    float Count(float /*p*/, int bin) const override
    {
        return counts_.at(bin);
    }

private:
    std::vector<float> counts_;
};

class FixedParameterHistogram final : public Histogram
{
public:
    FixedParameterHistogram() : Histogram("Fixed Parameter Histogram", false,
            kFixedParameter - 0.5f, kFixedParameter + 0.5f, 0.0f, 5.0f, 0.0f,
            8.0f)
    {
        data_sets_.push_back(
                std::make_unique < FixedHistogramData
                        > (std::vector<float> { 3.0f, 6.0f, 4.0f, 2.0f, 1.0f }, "blue", "Counts"));
    }

    const std::vector<std::unique_ptr<Histogram::Data>>& DataSets() const
            override
    {
        return data_sets_;
    }

    std::string Title(const float parameter) const override
    {
        return "Fixed Parameter Histogram (p = " + std::to_string(parameter)
                + ")";
    }

private:
    std::vector<std::unique_ptr<Histogram::Data>> data_sets_;
};

int main(int argc, char *argv[])
{
    if (argc > 1 && std::string(argv[1]) == "histogram")
    {
        FixedParameterHistogram histogram;
        histogram.Plot();
    } else
    {
        FixedParameterGraph graph;
        graph.Show();
    }
    return 0;
}
