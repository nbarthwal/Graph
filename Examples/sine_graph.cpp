#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>

static std::string SineLabel(float frequency)
{
    if (frequency == 1.0f)
    {
        return "sin(x)";
    }
    return "sin(" + std::to_string(frequency) + "x)";
}

class SineCurve final : public Graph::Data
{
public:
    SineCurve(float frequency, std::string color, bool point) : Graph::Data(
            0.0f, 6.28318f, std::move(color), SineLabel(frequency), point), frequency_(
            frequency)
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

class SineGraph final : public Graph
{
public:
    SineGraph() : Graph("Sine Graph", true, 0.5f, 2.0f, 0.0f, 6.28318f, -2.0f,
            2.0f)
    {
        curves_.push_back(std::make_unique < SineCurve > (1.0f, "blue", false));
        curves_.push_back(std::make_unique < SineCurve > (2.0f, "red", true));
    }

    const std::vector<std::unique_ptr<Graph::Data>>& DataSet() const override
    {
        return curves_;
    }

    std::string Title(const float parameter) const override
    {
        return "Sine Waves (p = " + std::to_string(parameter) + ")";
    }

private:
    std::vector<std::unique_ptr<Graph::Data>> curves_;
};

int main()
{
    SineGraph graph;
    graph.Plot();
    return 0;
}
