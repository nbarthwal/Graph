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


class Histogram
{
public:
    const bool Slider;
    const string WindowTitle;
    const float MinP;
    const float MaxP;
    const float MinX;
    const float MaxX;
    const float MinY;
    const float MaxY;

    Histogram(const string& title, const bool slider, const float min_p,
              const float max_p, const float min_x, const float max_x,
              const float min_y, const float max_y) : Slider(slider), WindowTitle(
              title), MinP(min_p), MaxP(max_p), MinX(min_x), MaxX(max_x),
              MinY(min_y), MaxY(max_y) { }

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

    [[nodiscard]] virtual const vector<unique_ptr<Data>>& DataSets() const = 0;
    [[nodiscard]] virtual string Title(float parameter) const = 0;
    GRAPH_API void Show(float parameter);
    GRAPH_API void Plot();
    virtual ~Histogram() = default;
};
