namespace GraphPlot;

public static class Graph
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

    public sealed record Canvas(string Title, string XLabel, string YLabel,
                                double MinimumX, double MaximumX,
                                double MinimumY, double MaximumY);

    public static void Plot(Canvas canvas, IReadOnlyList<Data> data) =>
        GraphApp.Plot(() => new GraphWindow(canvas, data));

    public static void Plot(Canvas canvas, DynamicData data) =>
        GraphApp.Plot(() => new GraphWindow(canvas, data));
}
