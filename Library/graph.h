#pragma once

#include <cstddef>
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


class Graph
{
public:

    struct Point
    {
        const float X;
        const float Y;
        Point(float x_, float y_);
    };

    class Segment
    {
    private:
        std::vector<unique_ptr<Point>> data;
        float min;
        float max;

    public:
        Segment(const std::vector<Point> &points);
        [[nodiscard]] float Min() const;
        [[nodiscard]] float Max() const;
        [[nodiscard]] int Size() const;
        const float X(size_t index) const;
        const float Y(size_t index) const;
        void Set(int, float);
    };

    class Data
    {
    public:
        const float MinX;
        const float MaxX;
        const string Color;
        const string Label;
        const bool Point;

        Data(float min_x, float max_x, const string& color,
             const string& label, bool point);

        [[nodiscard]] virtual const unique_ptr<Segment>& Value(float p) const = 0;
        virtual ~Data() = default;
    };

    const bool Slider;
    const string WindowTitle;
    const float MinP;
    const float MaxP;
    const float MinX;
    const float MaxX;
    const float MinY;
    const float MaxY;
    const std::vector<std::shared_ptr<Data>> Segments;

    Graph(const string& title, bool slider, float min_p, float max_p, float min_x,
          float max_x, float min_y, float max_y, const vector<shared_ptr<Data>>&);

    [[nodiscard]] virtual string Title(float parameter) const = 0;
    GRAPH_API void Show(float parameter);
    GRAPH_API void Plot();
    virtual ~Graph() = default;
};

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

        Data(string color, string label) : Color(std::move(color)),
                                                     Label(std::move(label)) {}

        // Number of bins in this histogram.
        [[nodiscard]] virtual std::size_t BinCount() const = 0;

        // Center of the bin on the x-axis.
        [[nodiscard]] virtual float BinCenter(std::size_t bin) const = 0;

        // Width of each bin on the x-axis.
        [[nodiscard]] virtual float BinWidth() const = 0;

        // Count in bin for slider value p.
        [[nodiscard]] virtual float Count(float p, std::size_t bin) const = 0;
        virtual ~Data() = default;
    };

    // Histogram data to display.
    [[nodiscard]] virtual const vector<unique_ptr<Data>>& DataSets() const = 0;
    // Title defines the histogram title for slider value p.
    [[nodiscard]] virtual string Title(float parameter) const = 0;
    GRAPH_API void Show(float parameter);
    GRAPH_API void Plot();
    virtual ~Histogram() = default;
};
