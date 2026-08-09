#include "graph.h"

#include <cmath>
#include <memory>
#include <string>
#include <vector>

class GaussianHistogramData final : public Histogram::Data
{
public:
    GaussianHistogramData(float mean, float sigma, std::string color) :
            mean_(mean), sigma_(sigma), color_(std::move(color))
    {
        for (std::size_t bin = 0; bin < kBinCount; ++bin)
        {
            centers_.push_back(
                    kMinCenter + static_cast<float>(bin) * kBinWidth);
        }
    }

    std::string Color() const override
    {
        return color_;
    }
    std::string Label() const override
    {
        return "N(" + std::to_string(mean_) + ", " + std::to_string(sigma_) + ")";
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
    std::string color_;
    std::vector<float> centers_;
};

class SliderHistogram final : public Histogram
{
public:
    SliderHistogram()
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

    float MaxX() const override
    {
        return 12.0f;
    }
    float MinX() const override
    {
        return 0.0f;
    }
    float MaxY() const override
    {
        return 12.0f;
    }
    float MinY() const override
    {
        return 0.0f;
    }
    float MaxP() const override
    {
        return 2.0f;
    }
    float MinP() const override
    {
        return 0.5f;
    }

    std::vector<const Histogram::Data*> DataSets() const override
    {
        std::vector<const Histogram::Data*> data_sets;
        data_sets.reserve(data_sets_.size());
        for (const auto &data_set : data_sets_)
        {
            data_sets.push_back(data_set.get());
        }
        return data_sets;
    }

    std::string Title() const override
    {
        return "Gaussian Mixture";
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
    Plot(histogram);
    return 0;
}
