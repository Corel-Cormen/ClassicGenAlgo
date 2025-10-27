#pragma once

#include <optional>
#include <variant>

#include "GeneticAlgorithmInterface.hpp"
#include "GeneticAlgorithmTypes.hpp"
#include "PyFunctionEvaluateAlgo.hpp"
#include "RandomCore.hpp"
#include "RandomPopulationFabric.hpp"

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

    GA::Types::GenomePopulation genomeVec;

    using GeneratePopulationStrategy = std::variant<RandomPopulationFabric>;
    std::optional<GeneratePopulationStrategy> generatePopulationStrategy;

    using EvaluatePopulationStategy = std::variant<PyFunctionEvaluateAlgo>;
    std::optional<EvaluatePopulationStategy> evaluatePopulationStrategy;

    bool initEnvironment();

    template<typename Strategy, typename... Args>
    void setGeneratePopulationStrategy(Args&&... args);

    template<typename Strategy, typename... Args>
    void setEvaluatePopulationStrategy(Args&&... args);

    bool createPopulation(const UiData& uiData);
    bool evaluatePopulation();
    void assessmentPopulation();
};
