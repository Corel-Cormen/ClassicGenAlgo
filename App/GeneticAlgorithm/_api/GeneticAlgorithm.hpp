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

    struct GenomePoint
    {
        std::vector<Genome> point;
        qreal value;
    };
    using GenomePopulation = std::vector<GenomePoint>;
    GenomePopulation genomeVec;

    bool initEnvironment();

    bool createPopulation(const UiData& uiData);
    bool evaluatePopulation();
    void assessmentPopulation();
};
