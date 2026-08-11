#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>

using namespace std;


const int N = 500;
const float Pi = 3.141593f;


class Curve: public Graph::Data
{
protected:
    [[nodiscard]] virtual float f(float) const = 0;

private:
    const float factor = 2.0f * Pi / static_cast<float>(N);
    static unique_ptr<Graph::Segment> gen()
    {
        vector<Graph::Point> points;
        for (int i = 0; i < N; ++i)
            points.emplace_back(static_cast<float>(i), 0.0f);
        return make_unique<Graph::Segment>(points);
    }

    mutable unique_ptr<Graph::Segment> segment = std::move(gen());

public:
    Curve(const string &color, const string &title, const bool b):
        Data(0.0f, static_cast<float>(N), color, title, b) { }

    [[nodiscard]] const unique_ptr<Graph::Segment>& Value(float k) const override
    {
        for (int i = 0; i < N; ++i)
            segment->Set(i, f(k * factor * segment->X(i)));
        return segment;
    }

    virtual ~Curve() = default;
};


class CosineCurve final : public Curve
{
public:
    CosineCurve(): Curve("blue", "cos(x)", false) { }
    ~CosineCurve() = default;

protected:
    [[nodiscard]] float f(float theta) const override
        { return std::cos(theta); }
};


class SineCurve final : public Curve
{
public:
    SineCurve(): Curve("red", "sin(x)", true) { }
    ~SineCurve() = default;

protected:
    [[nodiscard]] float f(float theta) const override
        { return std::sin(theta); }
};


class TrigonometryGraph final : public Graph
{
private:
    vector<unique_ptr<Graph::Data>> curves;

public:
    TrigonometryGraph(): Graph("Trigonometry", true, 1.0f, 5.0f, 0.0f,
                               static_cast<float>(N), -1.0f, 1.0f)
    {
        curves.push_back(std::make_unique<CosineCurve>());
        curves.push_back(std::make_unique<SineCurve>());
    }

    const vector<unique_ptr<Data>>& DataSet() const override
        { return curves; }

    [[nodiscard]] string Title(const float parameter) const override
        { return "Trigonometry (frequency = " + std::to_string(parameter) + ")"; }
};

int main()
{
    TrigonometryGraph graph;
    graph.Plot();
    return 0;
}
