#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>

constexpr float kTwoPi = 6.28318f;

class CosineCurve final : public Graph::Data
    {
    public:
        std::string Color() const override
        {
            return "blue";
        }
        std::string Label() const override
        {
            return "cos(x)";
        }
        float MaxX() const override
        {
            return kTwoPi;
        }
        float MinX() const override
        {
            return 0.0f;
        }
        float Value(float frequency, float x) const override
        {
            return std::cos(frequency * x);
        }
        bool Point() const override
        {
            return false;
        }
    };

    class SineCurve final : public Graph::Data
    {
    public:
        std::string Color() const override
        {
            return "red";
        }
        std::string Label() const override
        {
            return "sin(x)";
        }
        float MaxX() const override
        {
            return kTwoPi;
        }
        float MinX() const override
        {
            return 0.0f;
        }
        float Value(float frequency, float x) const override
        {
            return std::sin(frequency * x);
        }
        bool Point() const override
        {
            return true;
        }
    };

    class TrigonometryGraph final : public Graph
    {
    public:
        TrigonometryGraph(): Graph("Trigonometry", true)
        {
            curves_.push_back(std::make_unique<CosineCurve>());
            curves_.push_back(std::make_unique<SineCurve>());
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
            return 1.0f;
        }
        float MinY() const override
        {
            return -1.0f;
        }
        float MaxP() const override
        {
            return 5.0f;
        }
        float MinP() const override
        {
            return 1.0f;
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
            return "Trigonometry (frequency = " + std::to_string(parameter) + ")";
        }

    private:
        std::vector<std::unique_ptr<Graph::Data>> curves_;
};

int main()
{
    TrigonometryGraph graph;
    graph.Plot();
    return 0;
}
