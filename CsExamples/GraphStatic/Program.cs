using GraphPlot;

namespace CsExamples.GraphStatic;

internal static class Program
{
    private const int PointCount = 500;
    private const double Factor = 2 * Math.PI / PointCount;

    [STAThread]
    public static void Main()
    {
        var graph = new Graph("Static Graph Example", "Frequency", "Magnitude",
                              0, PointCount, -1, 1);
        IReadOnlyList<Graph.Data> data =
        [
            CreateCurve("blue", "Sin(x)", false, x => Math.Sin(Factor * x)),
            CreateCurve("red", "Cos(x)", true, x => Math.Cos(Factor * x)),
        ];
        graph.Plot(data);
    }

    private static Graph.Data CreateCurve(string color, string label, bool bullet,
        Func<double, double> function)
    {
        var points = new SortedDictionary<double, double>();
        for (int x = 0; x < PointCount; x++)
            points[x] = function(x);
        return new Graph.Data(color, label, bullet, points);
    }
}
