using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Layout;
using Avalonia.Themes.Fluent;
using ScottPlot;
using ScottPlot.Avalonia;

namespace GraphPlot;

internal sealed class HistogramWindow : Window
{
    private readonly Histogram.Canvas histogram;
    private readonly AvaPlot avaPlot = new();
    private readonly Histogram.DynamicData? dynamicData;
    private readonly TextBlock titleText = new()
    {
        FontSize = 20,
        FontWeight = Avalonia.Media.FontWeight.Bold,
        HorizontalAlignment = Avalonia.Layout.HorizontalAlignment.Center,
    };

    internal HistogramWindow(Histogram.Canvas histogram, IReadOnlyList<Histogram.Data> data)
    {
        this.histogram = histogram;
        Content = CreateLayout();
        Refresh(histogram.Title, data);
    }

    internal HistogramWindow(Histogram.Canvas histogram, Histogram.DynamicData data)
    {
        this.histogram = histogram;
        dynamicData = data;
        Content = CreateLayout();
        RefreshDynamic(data.MinimumParameter);
    }

    private Control CreateLayout()
    {
        Title = histogram.Title;
        Width = 900;
        Height = 700;
        avaPlot.MinHeight = 480;

        var layout = new Grid
        {
            Margin = new Avalonia.Thickness(12),
            RowDefinitions = new RowDefinitions("Auto,*,Auto"),
        };
        layout.Children.Add(titleText);
        Grid.SetRow(titleText, 0);
        layout.Children.Add(avaPlot);
        Grid.SetRow(avaPlot, 1);

        if (dynamicData is not null)
        {
            var slider = new Slider { Minimum = 0, Maximum = PlotHelpers.SliderSteps };
            slider.ValueChanged += (_, _) => RefreshDynamic(
                PlotHelpers.SliderToParameter(slider.Value, dynamicData.MinimumParameter,
                    dynamicData.MaximumParameter));
            layout.Children.Add(slider);
            Grid.SetRow(slider, 2);
        }
        return layout;
    }

    private void RefreshDynamic(double parameter) =>
        Refresh(dynamicData!.Title(parameter), dynamicData.Evaluate(parameter));

    private void Refresh(string title, IReadOnlyList<Histogram.Data> data)
    {
        titleText.Text = title;
        avaPlot.Plot.Clear();
        avaPlot.Plot.XLabel(histogram.XLabel);
        avaPlot.Plot.YLabel(histogram.YLabel);
        avaPlot.Plot.Axes.SetLimits(-0.5, histogram.Bins.Count - 0.5,
            histogram.MinimumY, histogram.MaximumY);

        for (int seriesIndex = 0; seriesIndex < data.Count; seriesIndex++)
        {
            Histogram.Data series = data[seriesIndex];
            IReadOnlyList<double> values = series.Count(histogram.Bins.Count);
            var bars = avaPlot.Plot.Add.Bars(values.Select((value, bin) => new Bar
            {
                Position = bin + (seriesIndex - (data.Count - 1) / 2d) * 0.8 / data.Count,
                Value = value,
                Size = 0.8 / data.Count,
                FillColor = PlotHelpers.ParseColor(series.Color),
                Label = series.Label,
            }).ToArray());
            bars.LegendText = series.Label;
        }

        avaPlot.Plot.Axes.Bottom.TickGenerator = new ScottPlot.TickGenerators.NumericManual(
            histogram.Bins.Select((label, index) => new Tick(index, label)).ToArray());
        if (data.Count > 0)
            avaPlot.Plot.ShowLegend(ScottPlot.Alignment.UpperRight);
        avaPlot.Refresh();
    }
}

internal sealed class HistogramApp : Application
{
    private static Func<Window>? windowFactory;

    public override void Initialize() => Styles.Add(new FluentTheme());

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            desktop.MainWindow = windowFactory?.Invoke();
        base.OnFrameworkInitializationCompleted();
    }

    // The window must be constructed after Avalonia initializes its platform,
    // so callers hand over a factory rather than a live window.
    public static void Plot(Func<Window> factory)
    {
        windowFactory = factory;
        AppBuilder.Configure<HistogramApp>().UsePlatformDetect().LogToTrace()
            .StartWithClassicDesktopLifetime([]);
    }
}
