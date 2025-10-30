#include <initializer_list>

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

    auto& uiData = uiDataHolder.getRef();
    uiData.selectAlgoNames = getAlgoName<SelectPopulationStrategy>();

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
    createSelectPopulationStrategy(uiData);

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

            // for (quint16 generation = 0U; generation < uiData.generations; ++generation)
            {
                status = evaluatePopulation();
                if (!status)
                {
                    qDebug() << "Evaluate population error";
                    // break;
                }

                status = selectPopulation(uiData);
                if (!status)
                {
                    qDebug() << "Select population error";
                    // break;
                }

                for (const auto& genome : genomeVec)
                {
                    qDebug() << genome.value;
                }
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

template<typename Strategy, typename... Args>
void GeneticAlgorithm::setSelectPopulationStrategy(Args&&... args)
{
    Strategy *strategyPtr = selectPopulationStrategy ?
                                std::get_if<Strategy>(&*selectPopulationStrategy) :
                                nullptr;
    if (!strategyPtr)
    {
        qDebug() << "Use new strategy for select population:" << typeid(Strategy).name();
        selectPopulationStrategy.emplace(std::in_place_type<Strategy>,
                                         std::forward<Args>(args)...);
    }
}

void GeneticAlgorithm::createSelectPopulationStrategy(const UiData& uiData)
{
    switch (uiData.selctAlgoIndex)
    {
    case 0U:
        setSelectPopulationStrategy<BestSelectionAlgo>();
        break;
    case 1U:
        setSelectPopulationStrategy<WorstSelectionAlgo>();
        break;
    default:
        qDebug() << "No choose select strategy";
    }
}

bool GeneticAlgorithm::createPopulation(const UiData &uiData)
{
    bool status = false;
    if (generatePopulationStrategy)
    {
        std::visit([&](auto& strategy) {
            status = strategy.generate(this->genomeVec, uiData);
        }, *generatePopulationStrategy);
    }
    return status;
}

bool GeneticAlgorithm::evaluatePopulation()
{
    bool status = false;
    if (evaluatePopulationStrategy)
    {
        std::visit([&](auto& strategy) {
            status = strategy.evaluate(this->genomeVec);
        }, *evaluatePopulationStrategy);
    }
    return status;
}

bool GeneticAlgorithm::selectPopulation(const UiData& uiData)
{
    bool status = false;
    if (selectPopulationStrategy)
    {
        std::visit([&](auto& strategy) {
            status = strategy.select(this->genomeVec, uiData);
        }, *selectPopulationStrategy);
    }
    return status;
}

template<typename Variant>
std::vector<QStringView> GeneticAlgorithm::getAlgoName()
{
    constexpr std::size_t N = std::variant_size_v<Variant>;
    return getAlgoName_impl<Variant>(std::make_index_sequence<N>{});
}

template<typename Variant, std::size_t... I>
std::vector<QStringView> GeneticAlgorithm::getAlgoName_impl(std::index_sequence<I...>)
{
    std::vector<QStringView> result{};
    (void) std::initializer_list<int> {
        (result.push_back(std::variant_alternative_t<I, Variant>::getAlgoName()), 0)...
    };
    return result;
}
