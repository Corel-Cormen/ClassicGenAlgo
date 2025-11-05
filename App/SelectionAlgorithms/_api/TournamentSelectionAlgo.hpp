#pragma once

#include "SelectionAlgoBase.hpp"

struct UiData;

class TournamentSelectionAlgo : public SelectionAlgoBase<TournamentSelectionAlgo>
{
public:
    bool select_impl(GA::Types::GenomePopulation &genomeVec, const UiData &uiData);

    static constexpr SelectionAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr SelectionAlgoId algoId{SelectionAlgoId::TOURNAMENT_SELECTION};

    static std::vector<bool> tournament(const GA::Types::GenomePopulation &genomeVec,
                                        const UiData &uiData);
};
