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

using namespace  std;


class Graph
{
public:

    class Point
    {
    private:
        float x;
        float y;

    public:
        float X() const;
        float Y() const;
        Point(const float x_, const float y_);
    };

    class Segment
    {
    private:
        std::vector<Point> data;
        float min;
        float max;

    public:
        Segment(const std::vector<Point>& points);
        [[nodiscard]] float Min() const;
        [[nodiscard]] float Max() const;
        int Size() const;
        const Point& operator[](size_t index) const;
    };

    class Data
    {
    public:
        const float MinX;
        const float MaxX;
        const std::string Color;
        const std::string Label;
        const bool Point;

        Data(float min_x, float max_x, std::string color, std::string label, bool point);

        virtual Segment& Value(float p) const = 0;
        virtual ~Data() = default;
    };

    const bool Slider;
    const std::string WindowTitle;
    const float MinP;
    const float MaxP;
    const float MinX;
    const float MaxX;
    const float MinY;
    const float MaxY;
    const std::vector<std::shared_ptr<Data>> Segments;

    Graph(const std::string &title, const bool slider, const float min_p,
          const float max_p, const float min_x, const float max_x,
          const float min_y, const float max_y,
          const std::vector<shared_ptr<Data>>& data);


    virtual std::string Title(const float parameter) const = 0;
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
    const float MinX;
    const float MaxX;
    const float MinY;
    const float MaxY;

    Histogram(const std::string &title, const bool slider, const float min_p,
            const float max_p, const float min_x, const float max_x,
            const float min_y, const float max_y) :
            Slider(slider), WindowTitle(title), MinP(min_p), MaxP(max_p),
            MinX(min_x), MaxX(max_x), MinY(min_y), MaxY(max_y) { }

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

    virtual const std::vector<std::unique_ptr<Data>>& DataSets() const = 0; // Histogram data to display.
    virtual std::string Title(const float parameter) const = 0; // Title defines the histogram title for slider value p.
    GRAPH_API void Show(const float parameter);
    GRAPH_API void Plot();
    virtual ~Histogram() = default;
};
