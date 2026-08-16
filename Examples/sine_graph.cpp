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

class SineGraph final : public Graph::DynamicPlot
{
public:
    SineGraph() : Graph::DynamicPlot("Sine Graph",
            Graph::Canvas(0.0f, 6.28318f, -2.0f, 2.0f), 0.5f, 2.0f)
    {
    }

    std::string Title(float parameter) const override
    {
        return "Sine Waves (p = " + std::to_string(parameter) + ")";
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
        constexpr float kMaxX = 6.28318f;
        Graph::points pts;
        for (int i = 0; i < kPointCount; ++i)
        {
            const float x = kMaxX * static_cast<float>(i)
                    / static_cast<float>(kPointCount - 1);
            pts[x] = p * std::sin(frequency * x);
        }
        return std::make_unique<Curve>(color, SineLabel(frequency),
                std::move(pts), bullet);
    }

    mutable std::vector<std::unique_ptr<Curve>> curves_;
    mutable std::vector<Graph::Data*> result_;
};

int main()
{
    SineGraph graph;
    graph.Show();
    return 0;
}
