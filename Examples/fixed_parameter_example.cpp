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
    FixedSineCurve(float frequency, std::string color, bool point) : Graph::Data(
            0.0f, kTwoPi, std::move(color),
            frequency == 1.0f ?
                    "sin(x)" : "sin(" + std::to_string(frequency) + "x)",
            point), frequency_(frequency)
    {
    }

    [[nodiscard]] const std::unique_ptr<Graph::Segment>& Value(float p) const override
    {
        for (int i = 0; i < kPointCount; ++i)
            segment_->Set(i, p * std::sin(frequency_ * segment_->X(i)));
        return segment_;
    }

private:
    static constexpr int kPointCount = 500;
    float frequency_;
    mutable std::unique_ptr<Graph::Segment> segment_ = BuildSegment();

    static std::unique_ptr<Graph::Segment> BuildSegment()
    {
        std::vector<Graph::Point> points;
        points.reserve(kPointCount);
        for (int i = 0; i < kPointCount; ++i)
            points.emplace_back(static_cast<float>(i), 0.0f);
        return std::make_unique<Graph::Segment>(points);
    }
};

class FixedParameterGraph final : public Graph
{
public:
    FixedParameterGraph() : Graph("Fixed Parameter Graph", false,
            kFixedParameter, kFixedParameter, 0.0f, kTwoPi, -1.5f, 1.5f)
    {
        curves_.push_back(
                std::make_unique < FixedSineCurve > (1.0f, "blue", false));
        curves_.push_back(
                std::make_unique < FixedSineCurve > (2.0f, "red", true));
    }

    const std::vector<std::unique_ptr<Graph::Data>>& DataSet() const override
    {
        return curves_;
    }

    std::string Title(const float parameter) const override
    {
        return "Fixed Parameter Graph (p = " + std::to_string(parameter) + ")";
    }

private:
    std::vector<std::unique_ptr<Graph::Data>> curves_;
};

class FixedHistogramData final : public Histogram::Data
{
public:
    FixedHistogramData(std::vector<float> centers, float bin_width,
            std::vector<float> counts, std::string color, std::string label) : Histogram::Data(
            std::move(color), std::move(label)), centers_(std::move(centers)), bin_width_(
            bin_width), counts_(std::move(counts))
    {
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
};

class FixedParameterHistogram final : public Histogram
{
public:
    FixedParameterHistogram() : Histogram("Fixed Parameter Histogram", false,
            kFixedParameter - 0.5f, kFixedParameter + 0.5f, 0.0f, 6.0f, 0.0f,
            8.0f)
    {
        data_sets_.push_back(
                std::make_unique < FixedHistogramData
                        > (std::vector<float> { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f }, 1.0f, std::vector<
                                float> { 3.0f, 6.0f, 4.0f, 2.0f, 1.0f }, "blue", "Counts"));
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
        graph.Plot();
    }
    return 0;
}
