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


namespace Histogram
{
    typedef vector<int> Points;


    class SliderPlot
    {
    protected:
        [[nodiscard]] virtual string Title(float parameter) const = 0;

    public:
        struct Data
        {
            const string Color;
            const string Label;
            const Points* Point;
            Data(const string& color, const string& label);
            virtual ~Data() = default;
        };

        const string WindowTitle;
        const float MinP;
        const float MaxP;
        const int Count;

        SliderPlot(const string&, const float min_p, const float max_p,
                   const vector<Data>&);
        GRAPH_API void Show(float) const;
    };


    class DynamicPlot
    {
    protected:
        [[nodiscard]] virtual string Title(float parameter) const = 0;

    public:
        const string WindowTitle;
        const float MinP;
        const float MaxP;
        const int Count;

        Graph(const string& title, const vector<Data>& data,
              const float min_p, const float max_p);

        GRAPH_API void Show(float) const;
        GRAPH_API void Update(float p, int pos, int n);
    };


    class Plot
    {
    public:
        const string WindowTitle;

        Graph(const string& title, const Data& data);
    };


    void Show(SliderPlot&);
    void Show(DynamicPlot&);
    void Show(Plot&);
};
