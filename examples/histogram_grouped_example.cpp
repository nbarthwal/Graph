#include "graph.h"

#include <cmath>
#include <memory>
#include <string>
#include <vector>

class StaticHistogramData final : public HistogramData
{
public:
    StaticHistogramData(
        std::vector<float> centers,
        float bin_width,
        std::vector<float> counts,
        std::string color)
        : centers_(std::move(centers)),
          bin_width_(bin_width),
          counts_(std::move(counts)),
          color_(std::move(color))
    {
    }

    std::string Color() const override { return color_; }
    std::size_t BinCount() const override { return centers_.size(); }
    float BinCenter(std::size_t bin) const override { return centers_.at(bin); }
    float BinWidth() const override { return bin_width_; }
    float Count(float /*p*/, std::size_t bin) const override { return counts_.at(bin); }

private:
    std::vector<float> centers_;
    float bin_width_;
    std::vector<float> counts_;
    std::string color_;
};

class GroupedHistogram final : public Histogram
{
public:
    GroupedHistogram()
    {
        data_sets_.push_back(std::make_unique<StaticHistogramData>(
            std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f},
            1.0f,
            std::vector<float>{2.0f, 5.0f, 8.0f, 4.0f, 1.0f},
            "blue"));

        data_sets_.push_back(std::make_unique<StaticHistogramData>(
            std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f},
            1.0f,
            std::vector<float>{1.0f, 3.0f, 6.0f, 7.0f, 2.0f},
            "orange"));
    }

    float MaxX() const override { return 6.0f; }
    float MinX() const override { return 0.0f; }
    float MaxY() const override { return 10.0f; }
    float MinY() const override { return 0.0f; }
    float MaxP() const override { return 1.0f; }
    float MinP() const override { return 1.0f; }

    std::vector<const HistogramData*> DataSets() const override
    {
        std::vector<const HistogramData*> data_sets;
        data_sets.reserve(data_sets_.size());
        for (const auto& data_set : data_sets_) {
            data_sets.push_back(data_set.get());
        }
        return data_sets;
    }

private:
    std::vector<std::unique_ptr<HistogramData>> data_sets_;
};

int main()
{
    GroupedHistogram histogram;
    PlotHistogram(histogram);
    return 0;
}
