#include <cmath>
#include <graph.h>
#include <histogram.h>
#include <memory>
#include <string>
#include <vector>


using namespace std;

/*
    class Data
    {
    private:
        const counts Values;

    public:
        const string Color;
        const string Label;

        Data(const string& color, const string& label, const counts& values):
            Color(color), Label(label), Values(values) { }

        counts Count(const int) const;
    };

    class DynamicData
    {
    public:
        const float MinP;
        const float MaxP;

        DynamicData(const float minP, const float maxP):
            MinP(minP), MaxP(maxP) { }
        virtual ~DynamicData();
        [[nodiscard]] virtual string Title(float parameter) const = 0;
        [[nodiscard]] virtual DataFrame Eval(float) = 0;
    };
*/
// GaussianHistogramData(float mean, float sigma, std::string color) 

Histogram::Data GaussianData(float mean, float sigma, const string& color)
{

}

class GaussianHistogramData final : public Histogram::DynamicData
{
public:
    GaussianHistogramData(float mean, float sigma, std::string color) : Histogram::Data(
            std::move(color),
            "N(" + std::to_string(mean) + ", " + std::to_string(sigma) + ")",
            kSize), mean_(mean), sigma_(sigma)
    {
    }

    float Count(float p, int bin) const override
    {
        const float x = static_cast<float>(bin) + 0.5f;
        const float exponent = -((x - mean_) * (x - mean_))
                / (2.0f * sigma_ * sigma_);
        return p * 10.0f * std::exp(exponent);
    }

private:
    static constexpr int kSize = 12;

    float mean_;
    float sigma_;
};

class SliderHistogram final : public Histogram::Base
{
public:
    SliderHistogram() : Histogram::Base(
            { "Gaussian Mixture", "Bin", "Count", 0.0f, 12.0f },
            true, 0.5f, 2.0f)
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

    const std::vector<std::unique_ptr<Histogram::Data>>& DataSets() const
            override
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
*/

int main()
{
    // SliderHistogram histogram;
    // Histogram::Plot(histogram);
    return 0;
}
