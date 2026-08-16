#pragma once

#include <map>
#include <memory>
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
        Data(const string& color, const string& label, const points& pts,
             bool bullet):
            Color(color), Label(label), Points(pts), Bullet(bullet) { }
    };

    typedef vector<Data*>& DynamicData;

    struct Canvas
    {
        const float MinX;
        const float MaxX;
        const float MinY;
        const float MaxY;
        Canvas(const Canvas&) = default;

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
        virtual ~DynamicPlot();

        GRAPH_API void Show(float) const;
        GRAPH_API void Show() const;

        [[nodiscard]] virtual string Title(float parameter) const = 0;
        [[nodiscard]] virtual DynamicData Eval(float) const = 0;
    };

    void Plot(const string& title, const Canvas&, const vector<Data>&);
};
