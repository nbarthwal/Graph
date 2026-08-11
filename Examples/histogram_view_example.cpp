#include <graph.h>
#include <memory>
#include <string>
#include <vector>

class StaticHistogramData final : public Histogram::Data
{
public:
    StaticHistogramData(std::vector<float> centers, float bin_width,
            std::vector<float> counts, std::string color, std::string label) : Histogram::Data(
            std::move(color), std::move(label)), centers_(std::move(centers)), bin_width_(
            bin_width), counts_(std::move(counts))
    {
    }

    std::size_t BinCount() const override
    {
        return centers_.size();
    }
    float BinCenter(std::size_t bin) const override
    {
        return centers_.at(bin);
    }
    float BinWidth() const override
    {
        return bin_width_;
    }
    float Count(float /*p*/, std::size_t bin) const override
    {
        return counts_.at(bin);
    }

private:
    std::vector<float> centers_;
    float bin_width_;
    std::vector<float> counts_;
};

class GroupedHistogram final : public Histogram
{
public:
    GroupedHistogram() : Histogram("Grouped Histogram", true, 1.0f, 1.0f, 0.0f,
            6.0f, 0.0f, 10.0f)
    {
        data_sets_.push_back(
                std::make_unique < StaticHistogramData
                        > (std::vector<float> { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f }, 1.0f, std::vector<
                                float> { 2.0f, 5.0f, 8.0f, 4.0f, 1.0f }, "blue", "Series A"));

        data_sets_.push_back(
                std::make_unique < StaticHistogramData
                        > (std::vector<float> { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f }, 1.0f, std::vector<
                                float> { 1.0f, 3.0f, 6.0f, 7.0f, 2.0f }, "orange", "Series B"));
    }

    const std::vector<std::unique_ptr<Histogram::Data>>& DataSets() const
            override
    {
        return data_sets_;
    }

    std::string Title(const float /*parameter*/) const override
    {
        return "Grouped Histogram";
    }

private:
    std::vector<std::unique_ptr<Histogram::Data>> data_sets_;
};

int main()
{
    GroupedHistogram histogram;
    histogram.Show(1.0f);
    return 0;
}
