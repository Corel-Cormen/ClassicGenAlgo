#include "InwerseMutationAlgo.hpp"

InwerseMutationAlgo::InwerseMutationAlgo(RandomCore &randomRef) :
    MutationDecider<InwerseMutationAlgo>(randomRef)
{}

void InwerseMutationAlgo::mutation_impl(Genome &genome)
{
    auto sectionIdx = random.randTwo<size_t>(0, genome.size()-1);
    if (sectionIdx.has_value())
    {
        while (sectionIdx->first < sectionIdx->second)
        {
            bool tmp = genome[sectionIdx->first];
            genome[sectionIdx->first] = genome[sectionIdx->second];
            genome[sectionIdx->second] = tmp;
            ++sectionIdx->first;
            --sectionIdx->second;
        }
    }
}
