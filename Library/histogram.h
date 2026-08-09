#pragma once

#include <cstddef>
#include <string>
#include <vector>

class HistogramData
{
public:
    virtual std::string Color() const = 0;  // Color defines the fill color of the bars.
    virtual std::size_t BinCount() const = 0;  // Number of bins in this histogram.
    virtual float BinCenter(std::size_t bin) const = 0;  // Center of the bin on the x-axis.
    virtual float BinWidth() const = 0;  // Width of each bin on the x-axis.
    virtual float Count(float p, std::size_t bin) const = 0;  // Count in bin for slider value p.
    virtual ~HistogramData() = default;
};

class Histogram
{
public:
    virtual float MaxX() const = 0;  // MaxX defines the max of X-Axis for this histogram.
    virtual float MinX() const = 0;  // MinX defines the min of X-Axis for this histogram.
    virtual float MaxY() const = 0;  // MaxY defines the max of Y-Axis for this histogram.
    virtual float MinY() const = 0;  // MinY defines the min of Y-Axis for this histogram.
    virtual float MaxP() const = 0;  // MaxP defines the max of slider parameter.
    virtual float MinP() const = 0;  // MinP defines the min of slider parameter.
    virtual std::vector<const HistogramData*> DataSets() const = 0;  // Histogram data to display.
    virtual ~Histogram() = default;
};
