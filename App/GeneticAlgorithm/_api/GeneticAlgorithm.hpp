#pragma once

#include <vector>

#include "GeneticAlgorithmInterface.hpp"
#include "RandomCore.hpp"
#include "Genome.hpp"

class FaultsManagerInterface;
class FunctionObserver;
class UiDataHolderInterface;
struct UiData;

class GeneticAlgorithm : public GeneticAlgorithmInterface
{
public:
    explicit GeneticAlgorithm(FaultsManagerInterface &faultsManagerRef,
                              UiDataHolderInterface &uiDataHolderRef,
                              FunctionObserver &functionObserverRef);

    void initProperties() override;

    bool initPopulation() override;

private:
    FaultsManagerInterface &faultsManager;
    UiDataHolderInterface &uiDataHolder;
    FunctionObserver &functionObserver;
    RandomCore random;

    using FunctionPopulation = std::vector<std::vector<Genome>>;
    FunctionPopulation genomeVec;

    bool createPopulation(const UiData& uiData);
};
