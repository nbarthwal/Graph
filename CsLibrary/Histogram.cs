using Avalonia.Controls;
using Avalonia.Layout;
using ScottPlot;
using ScottPlot.Avalonia;

namespace GraphPlot;

public static class Histogram
{
    public sealed record Data(string Color, string Label, IReadOnlyList<double> Values)
    {
        public IReadOnlyList<double> Count(int binCount) => Values.Count >= binCount
            ? Values.Take(binCount).ToArray()
            : Values.Concat(Enumerable.Repeat(0d, binCount - Values.Count)).ToArray();
    }

    public abstract class DynamicData
    {
        protected DynamicData(double minimumParameter, double maximumParameter)
        {
            MinimumParameter = minimumParameter;
            MaximumParameter = maximumParameter;
        }

        public double MinimumParameter { get; }
        public double MaximumParameter { get; }
        public abstract string Title(double parameter);
        public abstract IReadOnlyList<Data> Evaluate(double parameter);
    }

    public sealed record Canvas(string Title, string XLabel, string YLabel,
                                IReadOnlyList<string> Bins,
                                double MinimumY, double MaximumY);

    public HistogramWindow Plot(IReadOnlyList<Data> data) => new(this, data);
    public HistogramWindow Plot(DynamicData data) => new(this, data);

    public static void Plot(Canvas canvas, IReadOnlyList<Data> data)
    {
        HistogramApp.Update(new HistogramWindow(canvas, data));
        HistogramApp.Plot();
    }
    public static void Plot(Canvas canvas, Graph.DynamicData data)
    {
        HistogramApp.Update(new HistogramWindow(canvas, data));
        HistogramApp.Plot();
    }
}
