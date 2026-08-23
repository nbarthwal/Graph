using Avalonia.Controls;
using Avalonia.Layout;
using ScottPlot;
using ScottPlot.Avalonia;

namespace GraphPlot;

public sealed class Histogram
{
    public sealed record Data(string Color, string Label, IReadOnlyList<double> Values)
    {
        public IReadOnlyList<double> Count(int binCount) => Values.Count >= binCount
            ? Values.Take(binCount).ToArray()
            : Values.Concat(Enumerable.Repeat(0d, binCount - Values.Count)).ToArray();
    }

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

    public Histogram(string title, string xLabel, string yLabel,
        IReadOnlyList<string> bins, double minimumY, double maximumY)
    {
        Title = title;
        XLabel = xLabel;
        YLabel = yLabel;
        Bins = bins;
        MinimumY = minimumY;
        MaximumY = maximumY;
    }

    public string Title { get; }
    public string XLabel { get; }
    public string YLabel { get; }
    public IReadOnlyList<string> Bins { get; }
    public double MinimumY { get; }
    public double MaximumY { get; }

    public HistogramWindow Plot(IReadOnlyList<Data> data) => new(this, data);
    public HistogramWindow Plot(DynamicData data) => new(this, data);
}

public sealed class HistogramWindow : Window
{
    private readonly Histogram histogram;
    private readonly AvaPlot avaPlot = new();
    private readonly Histogram.DynamicData? dynamicData;
    private readonly TextBlock titleText = new()
    {
        FontSize = 20,
        FontWeight = Avalonia.Media.FontWeight.Bold,
        HorizontalAlignment = Avalonia.Layout.HorizontalAlignment.Center,
    };

    internal HistogramWindow(Histogram histogram, IReadOnlyList<Histogram.Data> data)
    {
        this.histogram = histogram;
        Content = CreateLayout();
        Refresh(histogram.Title, data);
    }

    internal HistogramWindow(Histogram histogram, Histogram.DynamicData data)
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
