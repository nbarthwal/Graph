using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Themes.Fluent;
using GraphPlot;

namespace CsExamples.GraphStatic;

public sealed class App : Application
{
    public override void Initialize() => Styles.Add(new FluentTheme());

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            const int pointCount = 500;
            const double factor = 2 * Math.PI / pointCount;
            var graph = new Graph("Static Graph Example", "Frequency", "Magnitude",
                0, pointCount, -1, 1);
            IReadOnlyList<Graph.Data> data =
            [
                CreateCurve("blue", "Sin(x)", false, x => Math.Sin(factor * x)),
                CreateCurve("red", "Cos(x)", true, x => Math.Cos(factor * x)),
            ];
            desktop.MainWindow = graph.Plot(data);
        }

        base.OnFrameworkInitializationCompleted();
    }

    private static Graph.Data CreateCurve(string color, string label, bool bullet,
        Func<double, double> function)
    {
        var points = new SortedDictionary<double, double>();
        for (int x = 0; x < 500; x++)
            points[x] = function(x);
        return new Graph.Data(color, label, bullet, points);
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
