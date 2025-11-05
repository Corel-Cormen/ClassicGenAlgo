#pragma once

#include "CommonFunctions.hpp"
#include "MutationAlgoBase.hpp"
#include "RandomCore.hpp"
#include "UiData.h"

template <typename Base>
class MutationDecider : public MutationAlgoBase<MutationDecider<Base>>
{
public:
    explicit MutationDecider(RandomCore &randomRef) :
        MutationAlgoBase<MutationDecider<Base>>(),
        random{randomRef}
    {}

    bool mutation_impl(GA::Types::GenomePopulation &genomVec,
                       const UiData& uiData);

    static constexpr MutationAlgoId getAlgoName_impl() { return Base::getAlgoName_impl(); }

protected:
    void mutation_impl(Genome &genome);

    RandomCore &random;
};

template <typename Base>
bool MutationDecider<Base>::mutation_impl(GA::Types::GenomePopulation &genomVec,
                                          const UiData& uiData)
{
    const quint32 mutationIndex = (uiData.eliteStrategyEnable == true) ?
                                      uiData.eliteStrategyQuantity :
                                      0U;
    for (size_t idx = mutationIndex; idx < genomVec.size(); ++idx)
    {
        for (auto& genome : genomVec[idx].point)
        {
            const qreal mutationProbability = random.randUnit<qreal>();
            if (CommonFunc::lessEqThan(mutationProbability, uiData.mutationPropablity))
            {
                static_cast<Base*>(this)->mutation_impl(genome);
            }
        }
    }

    return true;
}


