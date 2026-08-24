using ScottPlot;

namespace GraphPlot;

internal static class PlotHelpers
{
    public const int SliderSteps = 1000;

    // ScottPlot clips legend labels to a box sized from font metrics. On macOS the
    // default font draws above that box, so padding alone cannot fix top clipping.
    private const float LegendTopPadding = 14;
    private const float LegendSymbolHeight = 26;

    public static void ShowLegend(Plot plot)
    {
        Legend legend = plot.ShowLegend(Alignment.UpperRight);
        legend.FontName = "Arial";
        legend.SymbolHeight = LegendSymbolHeight;
        legend.Padding = new PixelPadding(legend.Padding.Left, legend.Padding.Right,
            legend.Padding.Bottom, LegendTopPadding);
    }

    public static double SliderToParameter(double sliderValue, double minimum, double maximum) =>
        minimum + sliderValue / SliderSteps * (maximum - minimum);

    public static Color ParseColor(string color) => color.Trim().ToLowerInvariant() switch
    {
        "red" or "r" => Colors.Red,
        "blue" or "b" => Colors.Blue,
        "green" or "g" => Colors.Green,
        "orange" => Colors.Orange,
        "purple" => Colors.Purple,
        "black" or "k" => Colors.Black,
        "white" or "w" => Colors.White,
        "cyan" => Colors.Cyan,
        "magenta" => Colors.Magenta,
        "yellow" => Colors.Yellow,
        _ => Color.FromHex(color),
    };

    public static double[] Linspace(double minimum, double maximum, int count)
    {
        if (count < 0)
            throw new ArgumentOutOfRangeException(nameof(count));
        if (count == 0)
            return [];
        if (count == 1)
            return [minimum];

        var values = new double[count];
        double step = (maximum - minimum) / (count - 1);
        for (int i = 0; i < count; i++)
            values[i] = minimum + i * step;
        return values;
    }
}
