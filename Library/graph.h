#pragma once

#include <cstddef>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(__CYGWIN__)
#  ifdef GRAPH_PLOT_BUILD_DLL
#    define GRAPH_API __declspec(dllexport)
#  else
#    define GRAPH_API __declspec(dllimport)
#  endif
#else
#  define GRAPH_API __attribute__((visibility("default")))
#endif

class Curve
{
public:
    virtual float MaxX() const = 0;  // MaxX defines the max of X-Axis for this curve.
    virtual float MinX() const = 0;  // MinX defines the min of X-Axis for this curve.
    virtual std::string Color() const = 0;  // Color defines the color of the curve.
    virtual float Value(float p, float x) const = 0;  // Value(p, x) returns y at x for slider value p.
    virtual bool Point() const = 0;  // Point defines if the curve should be plotted as a line or point.
    virtual ~Curve() = default;
};

class Graph
{
public:
    virtual float MaxX() const = 0;  // MaxX defines the max of X-Axis for this graph.
    virtual float MinX() const = 0;  // MinX defines the min of X-Axis for this graph.
    virtual float MaxY() const = 0;  // MaxY defines the max of Y-Axis for this graph.
    virtual float MinY() const = 0;  // MinY defines the min of Y-Axis for this graph.
    virtual float MaxP() const = 0;  // MaxP defines the max of slider parameter.
    virtual float MinP() const = 0;  // MinP defines the min of slider parameter.
    virtual std::vector<const Curve*> Curves() const = 0;  // Curves defines the curves for this graph.
    virtual ~Graph() = default;
};

class HistogramData
{
public:
    virtual std::string Color() const = 0;  // Color defines the fill color of the bars.
    virtual std::size_t BinCount() const = 0;  // Number of bins in this histogram.
    virtual float BinCenter(std::size_t bin) const = 0;  // Center of the bin on the x-axis.
    virtual float BinWidth() const = 0;  // Width of each bin on the x-axis.
    virtual float Count(float p, std::size_t bin) const = 0;  // Count in bin for slider value p.
    virtual ~HistogramData() = default;
};

class Histogram
{
public:
    virtual float MaxX() const = 0;  // MaxX defines the max of X-Axis for this histogram.
    virtual float MinX() const = 0;  // MinX defines the min of X-Axis for this histogram.
    virtual float MaxY() const = 0;  // MaxY defines the max of Y-Axis for this histogram.
    virtual float MinY() const = 0;  // MinY defines the min of Y-Axis for this histogram.
    virtual float MaxP() const = 0;  // MaxP defines the max of slider parameter.
    virtual float MinP() const = 0;  // MinP defines the min of slider parameter.
    virtual std::vector<const HistogramData*> DataSets() const = 0;  // Histogram data to display.
    virtual ~Histogram() = default;
};

GRAPH_API void Plot(Graph& graph);
GRAPH_API void ShowGraph(Graph& graph, float parameter);
GRAPH_API void PlotHistogram(Histogram& histogram);
GRAPH_API void ShowHistogram(Histogram& histogram, float parameter);
