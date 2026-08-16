#include <graph.h>
#include "trigonometry.h"


int main()
{
    const vector<Data> data = { SinCurve(1.0) , CosCurve(1.0) };
    Graph::Plot("Static Graph Exampple", canvas, data);
    return 0;
}
