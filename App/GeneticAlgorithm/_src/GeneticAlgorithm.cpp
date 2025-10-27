#include <QDebug>

#include "FaultsManagerInterface.hpp"
#include "FunctionObserver.hpp"
#include "GeneticAlgorithm.hpp"
#include "HypersphereFunction.hpp"
#include "PyInterface.hpp"
#include "UiDataHolderInterface.hpp"

GeneticAlgorithm::GeneticAlgorithm(FaultsManagerInterface &faultsManagerRef,
                                   UiDataHolderInterface &uiDataHolderRef,
                                   FunctionObserver &functionObserverRef,
                                   PyInterface &pyInterfaceRef) :
    faultsManager{faultsManagerRef},
    uiDataHolder{uiDataHolderRef},
    functionObserver{functionObserverRef},
    pyInterface{pyInterfaceRef}
{}

bool GeneticAlgorithm::setupInitial()
{
    uiDataHolder.resetDefault();
    functionObserver.subscribe(std::make_unique<HypersphereFunction>());
    return initEnvironment();
}

void GeneticAlgorithm::deinit()
{
    pyInterface.stopPython();
}

bool GeneticAlgorithm::initEnvironment()
{
    const bool result = pyInterface.startPython();
    if (!result)
    {
        qDebug() << "Run Python interpreter error";
        faultsManager.updateFault(Faults::RUN_PYTHON_ERROR, true);
    }
    return result;
}

bool GeneticAlgorithm::calculate()
{
    const UiData& uiData = uiDataHolder.getRef();

    setGeneratePopulationStrategy<RandomPopulationFabric>(random);
    setEvaluatePopulationStrategy<PyFunctionEvaluateAlgo>(pyInterface);

    functionObserver.choseFunctionId(uiData.selectFunctionId);

    bool status = pyInterface.makeFunction(functionObserver.getSelectFuncName(),
                                           uiData.functionDimension);
    if (status)
    {
        random.init(uiData.randomSeed);
        status = createPopulation(uiData);
        if(status)
        {
            qDebug() << "Create population success";

            for (quint16 generation = 0U; generation < uiData.generations; ++generation)
            {
                status = evaluatePopulation();
                if (!status)
                {
                    qDebug() << "Evaluate population error";
                }

                assessmentPopulation();
            }
        }

        if (!status)
        {
            qDebug() << "Genetic algorithm error";
            faultsManager.updateFault(Faults::GENETIC_ALGORITHM_ERROR, true);
        }
    }
    else
    {
        qDebug() << "Make function caller error";
        faultsManager.updateFault(Faults::PYTHON_FUNC_CALLER_ERROR, true);
    }
    pyInterface.discardFunction();

    return status;
}

template<typename Strategy, typename... Args>
void GeneticAlgorithm::setGeneratePopulationStrategy(Args&&... args)
{
    Strategy *strategyPtr = generatePopulationStrategy ?
                                std::get_if<Strategy>(&*generatePopulationStrategy) :
                                nullptr;
    if (!strategyPtr)
    {
        qDebug() << "Use new strategy for generate population:" << typeid(Strategy).name();
        generatePopulationStrategy.emplace(std::in_place_type<Strategy>,
                                           std::forward<Args>(args)...);
    }
}

template<typename Strategy, typename... Args>
void GeneticAlgorithm::setEvaluatePopulationStrategy(Args&&... args)
{
    Strategy *strategyPtr = evaluatePopulationStrategy ?
                                std::get_if<Strategy>(&*evaluatePopulationStrategy) :
                                nullptr;
    if (!strategyPtr)
    {
        qDebug() << "Use new strategy for evaluate population:" << typeid(Strategy).name();
        evaluatePopulationStrategy.emplace(std::in_place_type<Strategy>,
                                           std::forward<Args>(args)...);
    }
}

bool GeneticAlgorithm::createPopulation(const UiData &uiData)
{
    bool result = false;
    if (generatePopulationStrategy)
    {
        std::visit([&](auto& strategy) {
            result = strategy.generate(this->genomeVec, uiData);
        }, *generatePopulationStrategy);
    }
    return result;
}

bool GeneticAlgorithm::evaluatePopulation()
{
    bool result = false;
    if (evaluatePopulationStrategy)
    {
        std::visit([&](auto& strategy) {
            result = strategy.evaluate(this->genomeVec);
        }, *evaluatePopulationStrategy);
    }
    return result;
}

void GeneticAlgorithm::assessmentPopulation()
{
    std::sort(genomeVec.begin(), genomeVec.end(),
              [](const auto &a, const auto &b) {
        return a.value < b.value;
    });
}
