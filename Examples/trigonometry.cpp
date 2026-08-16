#include <cmath>
#include <graph.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>

using namespace std;

const int N = 500;
const float Pi = 3.141593f;

class TrigonometryGraph final : public Graph::DynamicPlot
{
public:
    TrigonometryGraph() : Graph::DynamicPlot("Trigonometry",
            Graph::Canvas(0.0f, static_cast<float>(N), -1.0f, 1.0f), 1.0f, 5.0f)
    {
    }

    string Title(float parameter) const override
    {
        return "Trigonometry (frequency = " + to_string(parameter) + ")";
    }

    Graph::DynamicData Eval(float k) const override
    {
        curves_.clear();
        result_.clear();
        result_.reserve(2);

        const float factor = 2.0f * Pi / static_cast<float>(N);

        curves_.push_back(MakeCurve("blue", "cos(x)", false, k, factor,
                [](float theta) { return cos(theta); }));
        result_.push_back(curves_.back().get());

        curves_.push_back(MakeCurve("red", "sin(x)", true, k, factor,
                [](float theta) { return sin(theta); }));
        result_.push_back(curves_.back().get());

        return result_;
    }

private:
    struct Curve final : public Graph::Data
    {
        Curve(string color, string label, Graph::points pts, bool bullet) :
                Graph::Data(color, label, pts, bullet)
        {
        }
    };

    static unique_ptr<Curve> MakeCurve(const string& color, const string& label,
            bool bullet, float k, float factor,
            const function<float(float)>& f)
    {
        Graph::points pts;
        for (int i = 0; i < N; ++i)
        {
            const float x = static_cast<float>(i);
            pts[x] = f(k * factor * x);
        }
        return make_unique<Curve>(color, label, std::move(pts), bullet);
    }

    mutable vector<unique_ptr<Curve>> curves_;
    mutable vector<Graph::Data*> result_;
};

int main()
{
    TrigonometryGraph graph;
    graph.Show();
    return 0;
}
