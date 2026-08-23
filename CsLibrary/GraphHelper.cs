using Avalonia.Controls;
using Avalonia.Layout;
using ScottPlot.Avalonia;

namespace GraphPlot;

internal sealed class GraphWindow : Window
{
    private readonly Graph graph;
    private readonly AvaPlot avaPlot = new();
    private readonly Graph.DynamicData? dynamicData;
    private readonly IReadOnlyList<Graph.Data>? staticData;
    private readonly TextBlock titleText = new()
    {
        FontSize = 20,
        FontWeight = Avalonia.Media.FontWeight.Bold,
        HorizontalAlignment = Avalonia.Layout.HorizontalAlignment.Center,
    };

    internal GraphWindow(Graph graph, IReadOnlyList<Graph.Data> data)
    {
        this.graph = graph;
        staticData = data;
        Content = CreateLayout();
        Refresh(graph.Title, data);
    }

    internal GraphWindow(Graph graph, Graph.DynamicData data)
    {
        this.graph = graph;
        dynamicData = data;
        Content = CreateLayout();
        RefreshDynamic(data.MinimumParameter);
    }

    private Control CreateLayout()
    {
        Title = graph.Title;
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
            var slider = new Slider
            {
                Minimum = 0,
                Maximum = PlotHelpers.SliderSteps,
                Value = 0,
            };
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

    private void Refresh(string title, IReadOnlyList<Graph.Data> data)
    {
        titleText.Text = title;
        avaPlot.Plot.Clear();
        avaPlot.Plot.XLabel(graph.XLabel);
        avaPlot.Plot.YLabel(graph.YLabel);
        avaPlot.Plot.Axes.SetLimits(graph.MinimumX, graph.MaximumX, graph.MinimumY, graph.MaximumY);

        foreach (Graph.Data series in data)
        {
            double[] xs = series.Points.Keys.ToArray();
            double[] ys = series.Points.Values.ToArray();
            var scatter = avaPlot.Plot.Add.Scatter(xs, ys);
            scatter.LegendText = series.Label;
            scatter.Color = PlotHelpers.ParseColor(series.Color);
            scatter.LineWidth = series.Bullet ? 0 : 2;
            scatter.MarkerSize = series.Bullet ? 4 : 0;
        }

        if (data.Count > 0)
            avaPlot.Plot.ShowLegend(ScottPlot.Alignment.UpperRight);
        avaPlot.Refresh();
    }
}
