using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Themes.Fluent;
using GraphPlot;

namespace CsExamples.HistogramStatic;

public sealed class App : Application
{
    public override void Initialize() => Styles.Add(new FluentTheme());

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            var histogram = new Histogram("Static Histogram", string.Empty, "Count",
                ["A", "B", "C", "D", "E"], 0, 10);
            IReadOnlyList<Histogram.Data> data =
            [
                new Histogram.Data("blue", "Series A", [2, 5, 8, 4, 1, 2]),
                new Histogram.Data("orange", "Series B", [1, 3, 6, 7]),
            ];
            desktop.MainWindow = histogram.Plot(data);
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
