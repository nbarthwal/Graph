#pragma once

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


namespace Histogram
{
    struct Canvas
    {
        const string Title;
        const string XLabel;
        const string YLabel;
        const float MinY;
        const float MaxY;
        Canvas(const Canvas&) = default;

        Canvas(const string& title, const string& xLabel, const string& yLabel,
               const float minY, const float maxY):
            Title(title), XLabel(xLabel), YLabel(yLabel),
            MinY(minY), MaxY(maxY) { }
    };

    class Data
    {
    public:
        const string Color;
        const string Label;
        const int Size;

        Data(string color, string label, int size): Color(std::move(color)),
                                                    Label(std::move(label)),
                                                    Size(size) {}

        [[nodiscard]] virtual float Count(float p, int bin) const = 0;
        virtual ~Data() = default;
    };

    class Base
    {
    public:
        const bool Slider;
        const string WindowTitle;
        const string XLabel;
        const string YLabel;
        const float MinP;
        const float MaxP;
        const float MinY;
        const float MaxY;

        Base(const Canvas& canvas, const bool slider, const float min_p,
             const float max_p) :
            Slider(slider), WindowTitle(canvas.Title),
            XLabel(canvas.XLabel), YLabel(canvas.YLabel),
            MinP(min_p), MaxP(max_p),
            MinY(canvas.MinY), MaxY(canvas.MaxY) { }

        [[nodiscard]] virtual const vector<unique_ptr<Data>>& DataSets() const = 0;
        [[nodiscard]] virtual string Title(float parameter) const = 0;
        virtual ~Base() = default;
    };

    GRAPH_API void Plot(Base&);
    GRAPH_API void Show(Base&, float parameter);

}
