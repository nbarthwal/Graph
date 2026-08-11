#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>

use namespace;


class Curve: public Graph::Data
{
public:
    const static int N = 500;
    const static float Pi = 3.141593f;
protected:
    float f(float);

private:
    const float Max = 10.0f;
    const float Min = 0.0f;
    const float n = static_cast<float>(N);

    static unique_ptr<Graph::Segment> gen(int n)
    {
        vector<Graph::Point> points();
        for(int i=0; i<n; ++i)
            points.push_back(Point(static_cast<float>(i), 0.0f))
        return make_unique<Graph::Segment>(points);
    }
    unique_ptr<Graph::Segment> segment;

public:
    Curve(const string &color, const string &title, const bool b):
        Data(Min, Max, color, title, b), segment(std::move(gen(N)) { }

    const unique_ptr<Graph::Segment>& Value(float k) const override
    {
        for (int i = 0; i <= N; ++i)
            segment->Set(i, f(
                (2.0f * Pi * (k + 1.0f) * segment->X(i)) / n));
        return segment
    }
};

class CosineCurve final : public Curve
{
public:
    CosineCurve() : Curve("blue", "cos(x)", false) { }

protected:
    float f(float theta) const override
        { return std::cos(theta); }
};

class SineCurve final : public Curve
{
public:
    SineCurve(): Curve("red", "sin(x)", true) { }

protected:
    float f(float theta) const override
        { return std::sin(theta); }
};

class TrigonometryGraph final : public Graph
{
private:
    std::vector<std::unique_ptr<Graph::Data>> curves;

public:
    TrigonometryGraph() : Graph("Trigonometry", true, 1.0f, 5.0f, 0.0f, kTwoPi,
            -1.0f, 1.0f)
    {
        curves.push_back(std::make_unique<CosineCurve>());
        curves.push_back(std::make_unique<SineCurve>());
    }

    const std::vector<std::unique_ptr<Graph::Data>>& Curves() const override
    {
        return curves;
    }

    std::string Title(const float parameter) const override
    {
        return "Trigonometry (frequency = " + std::to_string(parameter) + ")";
    }
};

int main()
{
    TrigonometryGraph graph;
    graph.Plot();
    return 0;
}
