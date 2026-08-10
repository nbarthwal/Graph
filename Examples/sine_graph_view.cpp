#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>

class SineCurve final : public Graph::Data
{
public:
    SineCurve(float frequency, std::string color, bool point):
            frequency_(frequency), color_(std::move(color)), point_(point)
    {
    }

    float MaxX() const override
    {
        return 6.28318f;
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

class SineGraph final : public Graph
{
public:
    SineGraph(): Graph("Sine Graph", false)
    {
        curves_.push_back(std::make_unique < SineCurve > (1.0f, "blue", false));
        curves_.push_back(std::make_unique < SineCurve > (2.0f, "red", true));
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
    graph.Show(1.0f);
    return 0;
}
