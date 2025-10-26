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
        qDebug() << "Make Function Caller error";
        faultsManager.updateFault(Faults::PYTHON_FUNC_CALLER_ERROR, true);
    }
    pyInterface.discardFunction();

    return status;
}

bool GeneticAlgorithm::createPopulation(const UiData& uiData)
{
    bool status = true;

    genomeVec.clear();
    genomeVec.resize(uiData.populationQuantity);
    for (size_t idx = 0; idx < static_cast<size_t>(uiData.populationQuantity); ++idx)
    {
        genomeVec[idx].point.reserve(uiData.functionDimension);
        for (size_t i = 0; i < static_cast<size_t>(uiData.functionDimension); ++i)
        {
            Genome genome;
            if (!genome.createGenom(uiData.minSearchRange,
                                    uiData.maxSearchRange,
                                    uiData.precission))
            {
                status = false;
                break;
            }
            genome.initGenom(random);
            genomeVec[idx].point.push_back(std::move(genome));
        }
    }

    return status;
}

bool GeneticAlgorithm::evaluatePopulation()
{
    bool calcStatus = false;

    for (auto& genomAxisPoint : genomeVec)
    {
        std::vector<qreal> point;
        for (auto& genomPoint : genomAxisPoint.point)
        {
            point.push_back(genomPoint.val());
        }

        calcStatus = pyInterface.calcPoint(point, genomAxisPoint.value);
        if (!calcStatus)
        {
            break;
        }
    }

    return calcStatus;
}

void GeneticAlgorithm::assessmentPopulation()
{
    std::sort(genomeVec.begin(), genomeVec.end(),
              [](const GenomePoint &a, const GenomePoint &b) {
        return a.value < b.value;
    });
}
