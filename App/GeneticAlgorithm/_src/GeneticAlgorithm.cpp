#include <initializer_list>

#include <QDebug>

#include "AlgoLoggerInterface.hpp"
#include "FaultsManagerInterface.hpp"
#include "FunctionObserver.hpp"
#include "GeneticAlgorithm.hpp"
#include "PyInterface.hpp"
#include "UiDataHolderInterface.hpp"

GeneticAlgorithm::GeneticAlgorithm(FaultsManagerInterface &faultsManagerRef,
                                   UiDataHolderInterface &uiDataHolderRef,
                                   FunctionObserver &functionObserverRef,
                                   PyInterface &pyInterfaceRef,
                                   AlgoLoggerInterface &algoLoggerRef) :
    faultsManager{faultsManagerRef},
    uiDataHolder{uiDataHolderRef},
    functionObserver{functionObserverRef},
    pyInterface{pyInterfaceRef},
    algoLogger{algoLoggerRef}
{}

bool GeneticAlgorithm::setupInitial()
{
    uiDataHolder.resetDefault();

    auto& uiData = uiDataHolder.getRef();
    uiData.selectAlgoNames = getAlgoName<SelectPopulationStrategy>();
    uiData.crossoverAlgoNames = getAlgoName<CrossoverPopulationStrategy>();
    uiData.crossoverBinaryAlgoQuantity = 4U;
    uiData.mutationAlgoNames = getAlgoName<MutationPopulationStrategy>();
    uiData.mutationBinaryAlgoQuantity = 4U;

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
    createCrossoverPopulationStrategy(uiData);
    createMutationPopulationStrategy(uiData);

    bool status = pyInterface.makeFunction(functionObserver.getSelectFuncName(),
                                           uiData.functionDimension);
    if (status)
    {
        algoLogger.reset();
        pathResultSave = algoLogger.createResultPath();
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
                    break;
                }
                algoLogger.colectData(genomeVec);

                status = selectPopulation(uiData);
                if (!status)
                {
                    qDebug() << "Select population error";
                    break;
                }

                status = crossoverPopulation(uiData);
                if (!status)
                {
                    qDebug() << "Crossover population error";
                    break;
                }

                status = mutationPopulation(uiData);
                if (!status)
                {
                    qDebug() << "Mutation population error";
                    break;
                }
            }

            if(status)
            {
                qDebug() << "Calculate final result";
                status = evaluatePopulation();
                if(status)
                {
                    algoLogger.colectData(genomeVec);
                    const auto& minVec = algoLogger.min();
                    qDebug() << "min point =" << minVec[minVec.size()-1].point;
                    qDebug() << "min value =" << minVec[minVec.size()-1].value;

                    pyInterface.showPlot(minVec[minVec.size()-1],
                                         pathResultSave + "/function_plot.png",
                                         uiData.showCharts);
                    pyInterface.showStats(algoLogger.min(),
                                          algoLogger.mean(),
                                          algoLogger.standardDeviation(),
                                          pathResultSave + "/best_fitness.png",
                                          pathResultSave + "/best_fitness.csv",
                                          pathResultSave + "/avg_fitness.png",
                                          pathResultSave + "/avg_fitness.csv",
                                          uiData.showCharts);
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

template<typename Strategy, typename... Args>
void GeneticAlgorithm::setCrossoverPopulationStrategy(Args&&... args)
{
    Strategy *strategyPtr = crossoverPopulationStrategy ?
                                std::get_if<Strategy>(&*crossoverPopulationStrategy) :
                                nullptr;
    if (!strategyPtr)
    {
        qDebug() << "Use new strategy for crossover population:" << typeid(Strategy).name();
        crossoverPopulationStrategy.emplace(std::in_place_type<Strategy>,
                                            std::forward<Args>(args)...);
    }
}

template<typename Strategy, typename... Args>
void GeneticAlgorithm::setMutationPopulationStrategy(Args&&... args)
{
    Strategy *strategyPtr = mutationPopulationStrategy ?
                                std::get_if<Strategy>(&*mutationPopulationStrategy) :
                                nullptr;
    if (!strategyPtr)
    {
        qDebug() << "Use new strategy for mutation population:" << typeid(Strategy).name();
        mutationPopulationStrategy.emplace(std::in_place_type<Strategy>,
                                           std::forward<Args>(args)...);
    }
}

void GeneticAlgorithm::createSelectPopulationStrategy(const UiData& uiData)
{
    switch (uiData.selctAlgoIndex)
    {
    case static_cast<decltype(uiData.selctAlgoIndex)>(SelectionAlgoId::BEST_SELECTION):
        setSelectPopulationStrategy<BestSelectionAlgo>();
        break;
    case static_cast<decltype(uiData.selctAlgoIndex)>(SelectionAlgoId::ROULETTE_SELECTION):
        setSelectPopulationStrategy<RouletteSelectionAlgo>(random);
        break;
    case static_cast<decltype(uiData.selctAlgoIndex)>(SelectionAlgoId::TOURNAMENT_SELECTION):
        setSelectPopulationStrategy<TournamentSelectionAlgo>();
        break;
    default:
        qDebug() << "No choose select strategy";
    }
}

void GeneticAlgorithm::createCrossoverPopulationStrategy(const UiData& uiData)
{
    switch (uiData.crossoverAlgoIndex)
    {
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::SINGLE_POINT):
        setCrossoverPopulationStrategy<SinglePointCrossover>(random);
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::TWO_POINT):
        setCrossoverPopulationStrategy<TwoPointCrossover>(random);
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::UNIFORM):
        setCrossoverPopulationStrategy<UniformCrossover>(random);
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::DISCRETE):
        setCrossoverPopulationStrategy<DiscreteCrossover>(random);
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::ARITMETIC):
        setCrossoverPopulationStrategy<ArithmeticCrossover>(random);
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::LINEAR):
        setCrossoverPopulationStrategy<LinearCrossover>(std::get<PyFunctionEvaluateAlgo>(*evaluatePopulationStrategy));
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::BLEND_ALPHA):
        setCrossoverPopulationStrategy<BlendAlphaCrossover>(random);
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::BLEND_ALPHA_BETA):
        setCrossoverPopulationStrategy<BlendAlphaBetaCrossover>(random);
        break;
    case static_cast<decltype(uiData.crossoverAlgoIndex)>(CrossoverAlgoId::AVERAGING):
        setCrossoverPopulationStrategy<AveragingCrossover>();
        break;
    default:
        qDebug() << "No choose crossover strategy";
    }
}

void GeneticAlgorithm::createMutationPopulationStrategy(const UiData& uiData)
{
    switch (uiData.mutationAlgoIndex) {
    case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::EDGE_MUTATION):
        setMutationPopulationStrategy<EdgeMutationAlgo>(random);
        break;
    case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::ONE_POINT_MUTATION):
        setMutationPopulationStrategy<OnePointMutationAlgo>(random);
        break;
    case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::TWO_POINT_MUTATION):
        setMutationPopulationStrategy<TwoPointMutationAlgo>(random);
        break;
    case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::INWERSE_MUTATION):
        setMutationPopulationStrategy<InwerseMutationAlgo>(random);
        break;
    case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::UNIFORM_MUTATION):
        setMutationPopulationStrategy<UniformMutationAlgo>(random);
        break;
    case static_cast<decltype(uiData.mutationAlgoIndex)>(MutationAlgoId::GAUSS_MUTATION):
        setMutationPopulationStrategy<GaussMutationAlgo>(random);
        break;
    default:
        qDebug() << "No choose mutation strategy";
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

bool GeneticAlgorithm::crossoverPopulation(const UiData& uiData)
{
    bool status = false;
    if (crossoverPopulationStrategy)
    {
        std::visit([&](auto& strategy) {
            status = strategy.crossover(this->genomeVec, uiData);
        }, *crossoverPopulationStrategy);
    }
    return status;
}

bool GeneticAlgorithm::mutationPopulation(const UiData& uiData)
{
    bool status = false;
    if (mutationPopulationStrategy)
    {
        std::visit([&](auto& strategy) {
            status = strategy.mutation(this->genomeVec, uiData);
        }, *mutationPopulationStrategy);
    }
    return status;
}

template<typename Variant>
std::vector<quint8> GeneticAlgorithm::getAlgoName()
{
    constexpr std::size_t N = std::variant_size_v<Variant>;
    return getAlgoName_impl<Variant>(std::make_index_sequence<N>{});
}

template<typename Variant, std::size_t... I>
std::vector<quint8> GeneticAlgorithm::getAlgoName_impl(std::index_sequence<I...>)
{
    std::vector<quint8> result{};
    (void) std::initializer_list<size_t> {
        (result.push_back(static_cast<quint8>(
             std::variant_alternative_t<I, Variant>::getAlgoName())), 0U)...
    };
    return result;
}
