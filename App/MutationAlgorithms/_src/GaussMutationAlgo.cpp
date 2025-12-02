#include "GaussMutationAlgo.hpp"
#include "CommonFunctions.hpp"

GaussMutationAlgo::GaussMutationAlgo(RandomCore &randomRef) :
    MutationDecider<GaussMutationAlgo>(randomRef),
    random{randomRef}
{}

void GaussMutationAlgo::mutation_impl(Genome &genome)
{
    const qreal newValue = genome.val() + random.randNormal();
    if (CommonFunc::moreThan(newValue, genome.getMinVal()) &&
        CommonFunc::lessThan(newValue, genome.getMaxVal()))
    {
        genome.setValue(newValue);
    }
}
