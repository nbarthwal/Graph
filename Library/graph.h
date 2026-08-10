#pragma once

#include <cstddef>
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

class Graph
{
public:
    const bool Slider;
    const std::string WindowTitle;
    const float MinP;
    const float MaxP;

    Graph(const std::string &title, const bool slider, const float min_p,
            const float max_p) :
            Slider(slider), WindowTitle(title), MinP(min_p), MaxP(max_p) { }

    class Data
    {
    public:
        const float MinX;
        const float MaxX;
        const std::string Color;
        const std::string Label;
        const bool Point;

        Data(float min_x, float max_x, std::string color, std::string label,
                bool point) :
                MinX(min_x), MaxX(max_x), Color(std::move(color)),
                Label(std::move(label)), Point(point) { }

        virtual float Value(float p, float x) const = 0; // Value(p, x) returns y at x for slider value p.
        virtual ~Data() = default;
    };

    virtual float MaxX() const = 0; // MaxX defines the max of X-Axis for this graph.
    virtual float MinX() const = 0; // MinX defines the min of X-Axis for this graph.
    virtual float MaxY() const = 0; // MaxY defines the max of Y-Axis for this graph.
    virtual float MinY() const = 0; // MinY defines the min of Y-Axis for this graph.
    virtual std::vector<const Data*> Curves() const = 0; // Curves defines the curves for this graph.
    virtual std::string Title(const float parameter) const = 0; // Title defines the graph title for slider value p.
    GRAPH_API void Show(const float parameter);
    GRAPH_API void Plot();
    virtual ~Graph() = default;
};


class Histogram
{
public:
    const bool Slider;
    const std::string WindowTitle;
    const float MinP;
    const float MaxP;

    Histogram(const std::string &title, const bool slider, const float min_p,
            const float max_p) :
            Slider(slider), WindowTitle(title), MinP(min_p), MaxP(max_p) { }

    class Data
    {
    public:
        const std::string Color;
        const std::string Label;

        Data(std::string color, std::string label) :
                Color(std::move(color)), Label(std::move(label)) { }

        virtual std::size_t BinCount() const = 0; // Number of bins in this histogram.
        virtual float BinCenter(std::size_t bin) const = 0; // Center of the bin on the x-axis.
        virtual float BinWidth() const = 0;  // Width of each bin on the x-axis.
        virtual float Count(float p, std::size_t bin) const = 0; // Count in bin for slider value p.
        virtual ~Data() = default;
    };

    virtual float MaxX() const = 0; // MaxX defines the max of X-Axis for this histogram.
    virtual float MinX() const = 0; // MinX defines the min of X-Axis for this histogram.
    virtual float MaxY() const = 0; // MaxY defines the max of Y-Axis for this histogram.
    virtual float MinY() const = 0; // MinY defines the min of Y-Axis for this histogram.
    virtual std::vector<const Data*> DataSets() const = 0; // Histogram data to display.
    virtual std::string Title(const float parameter) const = 0; // Title defines the histogram title for slider value p.
    GRAPH_API void Show(const float parameter);
    GRAPH_API void Plot();
    virtual ~Histogram() = default;
};
