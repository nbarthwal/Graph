namespace GraphPlot;

public class Graph
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

        public void Plot(Graph graph) => graph.Plot(this);
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

    public void Plot(IReadOnlyList<Data> data) =>
        GraphApp.Plot(() => new GraphWindow(this, data));

    public void Plot(DynamicData data) =>
        GraphApp.Plot(() => new GraphWindow(this, data));
}
