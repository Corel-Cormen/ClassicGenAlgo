#pragma once

#include <optional>
#include <variant>

#include "BestSelectionAlgo.hpp"
#include "GeneticAlgorithmInterface.hpp"
#include "GeneticAlgorithmTypes.hpp"
#include "PyFunctionEvaluateAlgo.hpp"
#include "RandomCore.hpp"
#include "RandomPopulationFabric.hpp"
#include "SinglePointCrossover.hpp"
#include "WorstSelectionAlgo.hpp"

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

    using SelectPopulationStrategy = std::variant<BestSelectionAlgo,
                                                  WorstSelectionAlgo>;
    std::optional<SelectPopulationStrategy> selectPopulationStrategy;

    using CrossoverPopulationStrategy = std::variant<SinglePointCrossover>;
    std::optional<CrossoverPopulationStrategy> crossoverPopulationStrategy;

    bool initEnvironment();

    template<typename Strategy, typename... Args>
    void setGeneratePopulationStrategy(Args&&... args);

    template<typename Strategy, typename... Args>
    void setEvaluatePopulationStrategy(Args&&... args);

    template<typename Strategy, typename... Args>
    void setSelectPopulationStrategy(Args&&... args);

    template<typename Strategy, typename... Args>
    void setCrossoverPopulationStrategy(Args&&... args);

    void createSelectPopulationStrategy(const UiData& uiData);

    bool createPopulation(const UiData& uiData);
    bool evaluatePopulation();
    bool selectPopulation(const UiData& uiData);
    bool crossoverPopulation(const UiData& uiData);

    template<typename Variant>
    static std::vector<QStringView> getAlgoName();

    template<typename Variant, std::size_t... I>
    static std::vector<QStringView> getAlgoName_impl(std::index_sequence<I...>);
};
