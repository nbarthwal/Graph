using GraphPlot;

namespace CsExamples.GraphDynamic;

public sealed class TrigonometryPlot : Graph.DynamicData
{
    private const int PointCount = 500;
    private const double Factor = 2 * Math.PI / PointCount;

    public TrigonometryPlot() : base(1, 5) { }

    public override string Title(double parameter) =>
        $"Trigonometry (frequency = {parameter:F2})";

    public override IReadOnlyList<Graph.Data> Evaluate(double parameter) =>
    [
        CreateCurve("blue", "Sin(x)", false, x => Math.Sin(parameter * Factor * x)),
        CreateCurve("red", "Cos(x)", true, x => Math.Cos(parameter * Factor * x)),
    ];

    private static Graph.Data CreateCurve(string color, string label, bool bullet,
        Func<double, double> function)
    {
        var points = new SortedDictionary<double, double>();
        for (int x = 0; x < PointCount; x++)
            points[x] = function(x);
        return new Graph.Data(color, label, bullet, points);
    }
}

internal static class Program
{
    [STAThread]
    public static void Main()
    {
        var data = new TrigonometryPlot();
        var graph = new Graph("Dynamic Graph Example", "Frequency", "Magnitude",
                              0, 500, -1, 1);
        data.Plot(graph);
    }
}
