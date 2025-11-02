#pragma once

#include "CrossoverAlgoBase.hpp"
#include "UiData.h"

template <typename Base>
class CrossoverIndexBase : public CrossoverAlgoBase<CrossoverIndexBase<Base>>
{
public:
    bool crossover_impl(GA::Types::GenomePopulation &genomVec,
                        const UiData& uiData);

    static constexpr CrossoverAlgoId getAlgoName_impl() { return Base::getAlgoName_impl(); }

protected:
    bool crossover_impl(const UiData &uiData,
                        const GA::Types::GenomePoint &g1,
                        const GA::Types::GenomePoint &g2,
                        GA::Types::GenomePoint &out1,
                        GA::Types::GenomePoint &out2)
    {
        return static_cast<Base*>(this)->crossover_impl(uiData, g1, g2, out1, out2);
    }

private:
    using CrossIndex = std::pair<size_t, size_t>;
    static std::vector<CrossIndex> crossIndex(const size_t genomSize,
                                              const size_t maxSize);

    bool generateGenom(const GA::Types::GenomePopulation &genomVec,
                       const std::vector<CrossIndex> &indexLUT,
                       const UiData &uiData,
                       GA::Types::GenomePopulation &resultVec);

    void holdElite(const GA::Types::GenomePopulation &genomVec,
                   const size_t populationSize,
                   GA::Types::GenomePopulation &resultVec);
};

template <typename Base>
bool CrossoverIndexBase<Base>::crossover_impl(GA::Types::GenomePopulation &genomVec,
                                              const UiData &uiData)
{
    bool status = true;

    const auto indexLUT = crossIndex(genomVec.size(),
                                     uiData.populationQuantity);
    if (indexLUT.size() > 2U)
    {
        GA::Types::GenomePopulation resultVec;
        resultVec.reserve(uiData.populationQuantity);

        if(uiData.eliteStrategyEnable)
        {
            holdElite(genomVec,
                      uiData.eliteStrategyQuantity,
                      resultVec);
        }

        status = generateGenom(genomVec,
                               indexLUT,
                               uiData,
                               resultVec);
        if (status)
        {
            genomVec = std::move(resultVec);
        }
    }
    else
    {
        status = false;
    }

    return status;
}

template <typename Base>
std::vector<typename CrossoverIndexBase<Base>::CrossIndex>
CrossoverIndexBase<Base>::crossIndex(const size_t genomSize,
                                     const size_t maxSize)
{
    std::vector<CrossIndex> result{};
    if(genomSize > 2U)
    {
        for (size_t k = 1U; k < genomSize; ++k)
        {
            if ((k % 2U) == 0U)
            {
                for (size_t a = 0U; a < k; ++a)
                {
                    result.emplace_back(a, k);
                }
            }
            else
            {
                size_t a0 = k - 1U;
                result.emplace_back(a0, k);
                for (size_t a = 0U; (a + 1U) < k; ++a)
                {
                    result.emplace_back(a, k);
                }
            }

            if (result.size() >= maxSize)
            {
                break;
            }
        }
    }
    return result;
}

template <typename Base>
bool CrossoverIndexBase<Base>::generateGenom(const GA::Types::GenomePopulation &genomVec,
                                             const std::vector<CrossIndex> &indexLUT,
                                             const UiData &uiData,
                                             GA::Types::GenomePopulation &resultVec)
{
    bool status = true;

    while (resultVec.size() != uiData.populationQuantity)
    {
        for (const auto &index : indexLUT)
        {
            GA::Types::GenomePoint crossGenom1{};
            GA::Types::GenomePoint crossGenom2{};
            if(!crossover_impl(uiData,
                                genomVec[index.first],
                                genomVec[index.second],
                                crossGenom1,
                                crossGenom2))
            {
                status = false;
                break;
            }
            resultVec.push_back(std::move(crossGenom1));

            if ((resultVec.size() != (uiData.populationQuantity)) &&
                !crossGenom2.point.empty())
            {
                resultVec.push_back(std::move(crossGenom2));
            }
            if (resultVec.size() == uiData.populationQuantity)
            {
                break;
            }
        }

        if (!status)
        {
            break;
        }
    }

    return status;
}

template <typename Base>
void CrossoverIndexBase<Base>::holdElite(const GA::Types::GenomePopulation &genomVec,
                                         const size_t populationSize,
                                         GA::Types::GenomePopulation &resultVec)
{
    for (size_t idx = 0U; idx < populationSize; ++idx)
    {
        resultVec.push_back(genomVec[idx]);
    }
}


