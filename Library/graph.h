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
    typedef map<float, float> Points;

    struct Data
    {
        const string Color;
        const string Label;
        const bool Point;
        const int Size;
        GRAPH_API [[nodiscard]] virtual const Points* Points() const = 0;
        Data(const string& color, const string& label, const bool point);
        virtual ~Data() = default;
    };

    class SliderPlot
    {
    public:

        const string WindowTitle;
        const float MinP;
        const float MaxP;
        const float MinX;
        const float MaxX;
        const float MinY;
        const float MaxY;

        SliderPlot(const string& title,
                   const float minP, const float maxP,
                   const float minX, const float maxX,
                   const float minY, const float maxY);

        GRAPH_API void Show(float) const;
        GRAPH_API void Show();

        virtual ~SliderPlot() = default;

    protected:
        [[nodiscard]] virtual string Title(float parameter) const = 0;
        [[nodiscard]] virtual const vector<const Data*>
            Eval(float parameter) const = 0;
    };


    class DynamicPlot
    {
    public:
        const string WindowTitle;
        const float MinP;
        const float MaxP;
        const float MinX;
        const float MaxX;
        const float MinY;
        const float MaxY;
        const int Count;

        DynamicPlot(const string& title, const vector<Data>& data,
              const float minP, const float maxP,
              const float minX, const float maxX,
              const float minY, const float maxY);

        GRAPH_API void Show(float) const;
        GRAPH_API void Show();

        virtual ~DynamicPlot() = default;
        
    protected:
        GRAPH_API void Update(float p, int pos, float x);
        [[nodiscard]] virtual string Title(float parameter) const = 0;
    };


    class StaticPlot
    {
    public:
        const string WindowTitle;
        const float MinX;
        const float MaxX;
        const float MinY;
        const float MaxY;

        StaticPlot(const string& title, const Data&,
                   const float minX, const float maxX,
                   const float minY, const float maxY);

        virtual ~StaticPlot() = default;

        GRAPH_API void Show(float) const;
        GRAPH_API void Show();
    };
};
