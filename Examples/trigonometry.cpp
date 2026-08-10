#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>





class Curve final : public Graph::Data
{
protected:
    float f(const float);

private:
    constexpr int N = 500;
    constexpr int Max = 10f;
    constexpr int Max = 0f;
    constexpr float Pi = 3.141593f;
    constexpr float n = static_cast<float>(N);
    vector<Graph::Point> segment(N + 1, Point(0.0, 0.0));
    vector<Graph::Segments>>

    float theta(const float k, const float x)
    {
        return (2.0f * Pi * (k + 1.0f) * x) / n;
    }

    Graph::Point point(float k, int i)
    {
        const float x = static_cast<float>(i);
        const float y = f(theta(k, x));
        return Pooint(x, y);
    }

public:
    Curve(const string& color, const string& title, const bool b):
        Graph::Data(Min, Max, color, title, b) { }

    const Graph::Segment& Value(float k) const override
    {
        for(int i = 0 ; i <= N ; ++i)
            segment[i] = std::move(point(k, i));
        return reference
    }
};


class CosineCurve final : public Curve
{
public:
    CosineCurve(): Curve("blue", "cos(x)", false) { }

protected:
    float f(float theta) const override { return std::cos(theta); }
};


class SineCurve final : public Curve
{
public:
    SineCurve(): : Curve("red", "sin(x)", true) { }

protected:
    float f(float theta) const override { return std::sin(theta); }
};


class TrigonometryGraph final : public Graph
{
private:
    std::vector<std::unique_ptr<Graph::Data>> curves;

public:
    TrigonometryGraph(): Graph("Trigonometry", true, 1.0f, 5.0f, 0.0f, kTwoPi,
            -1.0f, 1.0f)
    {
        curves.push_back(std::make_unique<CosineCurve>());
        curves.push_back(std::make_unique<SineCurve>());
    }

    const std::vector<std::unique_ptr<Graph::Data>>& Curves() const override
        { return curves; }

    std::string Title(const float parameter) const override
        { return "Trigonometry (frequency = " + std::to_string(parameter) + ")"; }
};

int main()
{
    TrigonometryGraph graph;
    graph.Plot();
    return 0;
}
