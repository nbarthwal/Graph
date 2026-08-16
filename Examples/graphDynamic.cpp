#include <graph.h>
#include <memory>
#include "trigonometry.h"


class DynamicData final: public Graph::DynamacData
{
private:
    Data sinCurve(SinCurve(1.0f));
    Data cosCurve(CosCurve(1.0f));
    DataFrame result(2, nullptr);

public:
    DynamicData(): base(1.0, 5.0) { }

    DataFrame Eval(float parameter) override
    {
        Data sinCurve = SinCurve(parameter);
        Data cosCurve = CosCurve(parameter);
        result[0] = &sinCurve;
        result[1] = &cosCurve;
        return result;
    }

    string Title(float parameter) const override
    {
        return "Trigonometry (frequency = " + to_string(parameter) + ")";
    }
};


int main()
{
    DynamicData data;
    Graph::Plot("Dynamic Graph Exampple", canvas, data);
    return 0;
}
