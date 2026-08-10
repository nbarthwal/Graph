#include <cmath>
#include <graph.h>
#include <memory>
#include <string>
#include <vector>

class GaussianHistogramData final : public Histogram::Data
{
public:
    GaussianHistogramData(float mean, float sigma, std::string color) :
            Histogram::Data(std::move(color),
                    "N(" + std::to_string(mean) + ", " + std::to_string(sigma)
                            + ")"),
            mean_(mean), sigma_(sigma)
    {
        for (std::size_t bin = 0; bin < kBinCount; ++bin)
        {
            centers_.push_back(
                    kMinCenter + static_cast<float>(bin) * kBinWidth);
        }
    }

    std::size_t BinCount() const override
    {
        return kBinCount;
    }
    float BinCenter(std::size_t bin) const override
    {
        return centers_.at(bin);
    }
    float BinWidth() const override
    {
        return kBinWidth;
    }

    float Count(float p, std::size_t bin) const override
    {
        const float x = centers_.at(bin);
        const float exponent = -((x - mean_) * (x - mean_))
                / (2.0f * sigma_ * sigma_);
        return p * 10.0f * std::exp(exponent);
    }

private:
    static constexpr std::size_t kBinCount = 12;
    static constexpr float kMinCenter = 0.5f;
    static constexpr float kBinWidth = 1.0f;

    float mean_;
    float sigma_;
    std::vector<float> centers_;
};

class SliderHistogram final : public Histogram
{
public:
    SliderHistogram(): Histogram("Gaussian Mixture", true, 0.5f, 2.0f, 0.0f,
            12.0f, 0.0f, 12.0f)
    {
        data_sets_.push_back(
                std::make_unique < GaussianHistogramData
                        > (4.0f, 1.2f, "blue"));
        data_sets_.push_back(
                std::make_unique < GaussianHistogramData > (7.0f, 1.5f, "red"));
        data_sets_.push_back(
                std::make_unique < GaussianHistogramData
                        > (2.5f, 0.9f, "green"));
    }

    const std::vector<std::unique_ptr<Histogram::Data>>& DataSets() const override
    {
        return data_sets_;
    }

    std::string Title(const float parameter) const override
    {
        return "Gaussian Mixture (p = " + std::to_string(parameter) + ")";
    }

private:
    std::vector<std::unique_ptr<Histogram::Data>> data_sets_;
};

int main()
{
    SliderHistogram histogram;
    histogram.Plot();
    return 0;
}
