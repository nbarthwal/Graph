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
    private:
        class BaseGraph;
        unique_ptr<BaseGraph> baseGraph;

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

        [[nodiscard]] virtual string Title(float parameter) const = 0;
        [[nodiscard]] virtual vector<const Data*>&
                                   Eval(float parameter) const = 0;
        virtual ~SliderPlot() = default;
    };


    class DynamicPlot
    {
    private:
        class BaseGraph;
        unique_ptr<BaseGraph> baseGraph;

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

        GRAPH_API virtual ~DynamicPlot() = default;
        GRAPH_API [[nodiscard]] virtual string Title(float parameter) const = 0;
        
    protected:
        GRAPH_API void Update(float p, int pos, float x);
    };


    class StaticPlot
    {
    private:
        class BaseGraph;
        unique_ptr<BaseGraph> baseGraph;

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
