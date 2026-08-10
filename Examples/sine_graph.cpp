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
    SineCurve(float frequency, std::string color, bool point) :
            Graph::Data(0.0f, 6.28318f, std::move(color), SineLabel(frequency),
                    point),
            frequency_(frequency)
    {
    }

    float Value(float p, float x) const override
    {
        return p * std::sin(frequency_ * x);
    }

private:
    float frequency_;
};

class SineGraph final : public Graph
{
public:
    SineGraph(): Graph("Sine Graph", true)
    {
        curves_.push_back(std::make_unique<SineCurve>(1.0f, "blue", false));
        curves_.push_back(std::make_unique<SineCurve>(2.0f, "red", true));
    }

    float MaxX() const override
    {
        return 6.28318f;
    }
    float MinX() const override
    {
        return 0.0f;
    }
    float MaxY() const override
    {
        return 2.0f;
    }
    float MinY() const override
    {
        return -2.0f;
    }
    float MaxP() const override
    {
        return 2.0f;
    }
    float MinP() const override
    {
        return 0.5f;
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
