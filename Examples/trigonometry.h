#include <cmath>
#include <graph.h>

using namespace std;


const float N = static_cast<float>(500);
const float Pi = 3.141593f;
const float factor = 2.0f * Pi / N;


Data SinCurve(float frequency)
{
    Graph::points pts;
    for (int i = 0; i < N; ++i)
    {
        const float x = static_cast<float>(i);
        pts[x] = sin(k * factor * x);
    }
    return Graph::Data("blue", "Sin(x)", false, pts); 
}


Data CosCurve(float frequency)
{
    Graph::points pts;
    for (int i = 0; i < N; ++i)
    {
        const float x = static_cast<float>(i);
        pts[x] = sin(k * factor * x);
    }
    return Data("red", "Cosn(x)", true, pts); 
}


Graph::Canvas canvas(0.0f, N, , -1.0f, 1.0f);
