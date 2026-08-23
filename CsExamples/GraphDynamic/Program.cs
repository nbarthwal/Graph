using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Themes.Fluent;
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

public sealed class App : Application
{
    public override void Initialize() => Styles.Add(new FluentTheme());

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            var graph = new Graph("Dynamic Graph Example", "Frequency", "Magnitude",
                0, 500, -1, 1);
            desktop.MainWindow = graph.Plot(new TrigonometryPlot());
        }

        base.OnFrameworkInitializationCompleted();
    }
}

internal static class Program
{
    [STAThread]
    public static void Main(string[] args) =>
        BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);

    private static AppBuilder BuildAvaloniaApp() =>
        AppBuilder.Configure<App>().UsePlatformDetect().LogToTrace();
}
