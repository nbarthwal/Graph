#include "graph.h"

#include <cmath>
#include <memory>
#include <string>
#include <vector>

class SineCurve final : public Curve
{
public:
    SineCurve(float frequency, std::string color, bool point) :
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
    SineGraph()
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

    std::vector<const Curve*> Curves() const override
    {
        std::vector<const Curve*> curves;
        curves.reserve(curves_.size());
        for (const auto &curve : curves_)
        {
            curves.push_back(curve.get());
        }
        return curves;
    }

private:
    std::vector<std::unique_ptr<Curve>> curves_;
};

int main()
{
    SineGraph graph;
    ShowGraph(graph, 1.0f);
    return 0;
}
