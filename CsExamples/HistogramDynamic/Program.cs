using GraphPlot;

namespace CsExamples.HistogramDynamic;

public sealed class GaussianDynamicData : Histogram.DynamicData
{
    private const int BinCount = 10;

    public GaussianDynamicData() : base(.5, 2) { }

    public override string Title(double parameter) =>
        $"Gaussian Mixture (p = {parameter:F2})";

    public override IReadOnlyList<Histogram.Data> Evaluate(double parameter) =>
    [
        CreateData("blue", "Blue", 4, 1.2, parameter),
        CreateData("green", "Green", 7, 1.5, parameter),
        CreateData("red", "Red", 2.5, .9, parameter),
    ];

    private static Histogram.Data CreateData(string color, string label, double mean,
        double sigma, double parameter)
    {
        double[] values = Enumerable.Range(0, BinCount)
            .Select(bin =>
            {
                double x = bin + .5;
                double exponent = -Math.Pow(x - mean, 2) / (2 * sigma * sigma);
                return parameter * 10 * Math.Exp(exponent);
            })
            .ToArray();
        return new Histogram.Data(color, label, values);
    }
}

internal static class Program
{
    [STAThread]
    public static void Main()
    {
        var data = new GaussianDynamicData();
        var histogram = new Histogram("Dynamic Histogram", "Bin", "Count",
            ["Bin0", "Bin1", "Bin2", "Bin3", "Bin4", "Bin5", "Bin6", "Bin7",
             "Bin8", "Bin9"], 0, 12);
        data.Plot(histogram);
    }
}
