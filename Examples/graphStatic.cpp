#include <graph.h>
#include "trigonometry.h"


int main()
{
    const vector<Graph::Data> data = { SinCurve(1.0f), CosCurve(1.0f) };
    Graph::Plot("Static Graph Exampple", canvas, data);
    return 0;
}
