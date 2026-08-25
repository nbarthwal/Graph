using GraphPlot;

namespace CsExamples.HistogramStatic;

internal static class Program
{
    [STAThread]
    public static void Main()
    {
        var histogram = new Histogram("Static Histogram", string.Empty, "Count",
                                      ["A", "B", "C", "D", "E"], 0, 10);
        IReadOnlyList<Histogram.Data> data =
        [
            new Histogram.Data("blue", "Series A", [2, 5, 8, 4, 1, 2]),
            new Histogram.Data("orange", "Series B", [1, 3, 6, 7]),
        ];
        histogram.Plot(data);
    }
}
