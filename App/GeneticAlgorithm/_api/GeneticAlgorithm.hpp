#pragma once

#include <vector>

#include "GeneticAlgorithmInterface.hpp"
#include "RandomCore.hpp"
#include "Genome.hpp"

class FaultsManagerInterface;
class FunctionObserver;
class PyInterface;
class UiDataHolderInterface;
struct UiData;

class GeneticAlgorithm : public GeneticAlgorithmInterface
{
public:
    explicit GeneticAlgorithm(FaultsManagerInterface &faultsManagerRef,
                              UiDataHolderInterface &uiDataHolderRef,
                              FunctionObserver &functionObserverRef,
                              PyInterface &pyInterfaceRef);

    bool setupInitial() override;

    void deinit() override;

    bool calculate() override;

private:
    FaultsManagerInterface &faultsManager;
    UiDataHolderInterface &uiDataHolder;
    FunctionObserver &functionObserver;
    PyInterface &pyInterface;
    RandomCore random;

    using FunctionPopulation = std::vector<std::vector<Genome>>;
    FunctionPopulation genomeVec;

    bool createPopulation(const UiData& uiData);
    bool initEnvironment();
};
