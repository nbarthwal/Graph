using Avalonia.Controls;
using Avalonia.Layout;
using ScottPlot.Avalonia;

namespace GraphPlot;

public sealed class Graph
{
    public sealed record Data(string Color, string Label, bool Bullet,
        IReadOnlyDictionary<double, double> Points);

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

    public Graph(string title, string xLabel, string yLabel,
        double minimumX, double maximumX, double minimumY, double maximumY)
    {
        Title = title;
        XLabel = xLabel;
        YLabel = yLabel;
        MinimumX = minimumX;
        MaximumX = maximumX;
        MinimumY = minimumY;
        MaximumY = maximumY;
    }

    public string Title { get; }
    public string XLabel { get; }
    public string YLabel { get; }
    public double MinimumX { get; }
    public double MaximumX { get; }
    public double MinimumY { get; }
    public double MaximumY { get; }

    public GraphWindow Plot(IReadOnlyList<Data> data) => new(this, data);
    public GraphWindow Plot(DynamicData data) => new(this, data);
}
