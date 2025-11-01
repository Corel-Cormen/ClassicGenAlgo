#pragma once

#include <vector>
#include <utility>

#include "GeneticAlgorithmTypes.hpp"
#include "UiData.h"

struct UiData;

template <typename Base>
class CrossoverAlgoBase
{
public:
    bool crossover(GA::Types::GenomePopulation &genomVec, const UiData& uiData)
    {
        bool status = true;

        const auto indexLUT = crossIndex(genomVec.size(), uiData.populationQuantity);
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
                                   uiData.populationQuantity,
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

protected:
    bool crossover(const GA::Types::GenomePoint &g1,
                   const GA::Types::GenomePoint &g2,
                   GA::Types::GenomePoint &out)
    {
        return static_cast<Base*>(this)->crossover_impl(g1, g2, out);
    }

private:
    using CrossIndex = std::pair<size_t, size_t>;

    static std::vector<CrossIndex> crossIndex(const size_t genomSize, const size_t maxSize)
    {
        size_t len = 0;
        std::vector<CrossIndex> result{};
        if(genomSize > 2)
        {
            for (size_t k = 1U; k < genomSize; ++k)
            {
                if ((k % 2U) == 0U)
                {
                    for (size_t a = 0U; a < k; ++a)
                    {
                        result.emplace_back(a, k);
                        result.emplace_back(k, a);
                    }
                }
                else
                {
                    size_t a0 = k - 1U;
                    result.emplace_back(a0, k);
                    result.emplace_back(k, a0);
                    for (size_t a = 0U; (a + 1U) < k; ++a)
                    {
                        result.emplace_back(a, k);
                        result.emplace_back(k, a);
                        ++len;
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

    bool generateGenom(const GA::Types::GenomePopulation &genomVec,
                       const std::vector<CrossIndex> &indexLUT,
                       const size_t populationSize,
                       GA::Types::GenomePopulation &resultVec)
    {
        bool status = true;

        while (resultVec.size() != populationSize)
        {
            for (const auto &index : indexLUT)
            {
                GA::Types::GenomePoint crossGenom{};
                if(!crossover(genomVec[index.first], genomVec[index.second], crossGenom))
                {
                    status = false;
                    break;
                }
                resultVec.push_back(std::move(crossGenom));

                if(resultVec.size() == populationSize)
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

    void holdElite(const GA::Types::GenomePopulation &genomVec,
                   const size_t populationSize,
                   GA::Types::GenomePopulation &resultVec)
    {
        for (size_t idx = 0U; idx < populationSize; ++idx)
        {
            resultVec.push_back(genomVec[idx]);
        }
    }
};
