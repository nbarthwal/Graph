namespace GraphPlot;

public class Histogram
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

        public void Plot(Histogram canvas) => canvas.Plot(this);
    }

    public Histogram(string title, string xLabel, string yLabel,
                     IReadOnlyList<string> bins, double minimumY, double maximumY)
    {
        Title = title;
        XLabel = xLabel;
        YLabel = yLabel;
        Bins = bins;
        MinimumY = minimumY;
        MaximumY = maximumY;
    }

    public string Title { get; }
    public string XLabel { get; }
    public string YLabel { get; }
    public IReadOnlyList<string> Bins { get; }
    public double MinimumY { get; }
    public double MaximumY { get; }

    public void Plot(IReadOnlyList<Data> data) =>
        HistogramApp.Plot(() => new HistogramWindow(this, data));

    public void Plot(DynamicData data) =>
        HistogramApp.Plot(() => new HistogramWindow(this, data));
}
