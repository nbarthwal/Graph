#include <graph.h>
#include <vector>
#include "trigonometry.h"


class TrigonometryPlot final : public Graph::DynamicData
{
private:
    std::vector<CurveData> curves_;
    std::vector<Graph::Data*> result_;

public:
    TrigonometryPlot() : Graph::DynamicData(1.0f, 5.0f) { }

    Graph::DataFrame Eval(float parameter) override
    {
        curves_.clear();
        curves_.push_back(SinCurve(parameter));
        curves_.push_back(CosCurve(parameter));
        result_ = { &curves_[0], &curves_[1] };
        return result_;
    }

    string Title(float parameter) const override
    {
        return "Trigonometry (frequency = " + to_string(parameter) + ")";
    }
};


int main()
{
    TrigonometryPlot plot;
    Graph::Plot("Dynamic Graph Exampple", canvas, plot);
    return 0;
}
