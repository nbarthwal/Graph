#pragma once

#include <map>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(__CYGWIN__)
#  ifdef GRAPH_PLOT_BUILD_DLL
#    define GRAPH_API __declspec(dllexport)
#  else
#    define GRAPH_API
#  endif
#else
#  define GRAPH_API
#endif

using namespace std;


namespace Graph
{
    typedef map<float, float> points;

    class Data
    {
    public:
        const string Color;
        const string Label;
        const points Points;
        const bool Bullet;
        virtual ~Data() = default;

    protected:
        Data(const string& color, const string& label, bool bullet):
            Color(color), Label(label), Bullet(bullet) { }
    };

    typedef vector<Data*>& DynamicData;

    struct Canvas
    {
        const float MinX;
        const float MaxX;
        const float MinY;
        const float MaxY;
        Canvas(const Canvas&);

        Canvas(const float minX, const float maxX,
               const float minY, const float maxY):
            MinX(minX), MaxX(maxX), MinY(minY), MaxY(maxY) { }
    };

    class DynamicPlot
    {
    private:
        class DynamicPlotBase;
        unique_ptr<DynamicPlotBase> ptr;

    public:
        DynamicPlot(const string& title, const Canvas& canvas,
                    const float minP, const float maxP);

        GRAPH_API void Show(float) const;
        GRAPH_API void Show();

        [[nodiscard]] virtual string Title(float parameter) const = 0;
        [[nodiscard]] virtual const DynamicData Eval(float) const = 0;
    };

    //TODO: void Plot(const string& title, const Canvas&, const vector<Data>&);
};
