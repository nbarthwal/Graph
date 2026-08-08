#include "graph.h"
#include "graph_plot.h"

#include <cmath>
#include <memory>
#include <string>
#include <vector>

namespace {

constexpr float kTwoPi = 6.28318f;

class CosineCurve final : public Curve
{
public:
    std::string Color() const override { return "blue"; }
    float MaxX() const override { return kTwoPi; }
    float MinX() const override { return 0.0f; }
    float Value(float frequency, float x) const override { return std::cos(frequency * x); }
    bool Point() const override { return false; }
};

class SineCurve final : public Curve
{
public:
    std::string Color() const override { return "red"; }
    float MaxX() const override { return kTwoPi; }
    float MinX() const override { return 0.0f; }
    float Value(float frequency, float x) const override { return std::sin(frequency * x); }
    bool Point() const override { return true; }
};

class TrigonometryGraph final : public Graph
{
public:
    TrigonometryGraph()
    {
        curves_.push_back(std::make_unique<CosineCurve>());
        curves_.push_back(std::make_unique<SineCurve>());
    }

    float MaxX() const override { return kTwoPi; }
    float MinX() const override { return 0.0f; }
    float MaxY() const override { return 1.0f; }
    float MinY() const override { return -1.0f; }
    float MaxP() const override { return 5.0f; }
    float MinP() const override { return 1.0f; }

    std::vector<const Curve*> Curves() const override
    {
        std::vector<const Curve*> curves;
        curves.reserve(curves_.size());
        for (const auto& curve : curves_) {
            curves.push_back(curve.get());
        }
        return curves;
    }

private:
    std::vector<std::unique_ptr<Curve>> curves_;
};

}  // namespace

int main()
{
    TrigonometryGraph graph;
    Plot(graph);
    return 0;
}
