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

            // python test
            std::vector<qreal> pointVec;
            for(size_t i = 0; i < genomeVec.size(); ++i)
            {
                pointVec.push_back(0.0);
            }

            qreal val;
            bool calcStatus = pyInterface.calcPoint(pointVec, val);
            qDebug() << "py_test: calcPoint status =" << calcStatus << "val =" << val;
            // end python test

            // tutaj pętla for dla algorytmu dopóki uiData.generations nie zostanie osiągnięta
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
    genomeVec.resize(uiData.functionDimension);
    for (size_t dim = 0; dim < static_cast<size_t>(uiData.functionDimension); ++dim)
    {
        genomeVec[dim].reserve(uiData.populationQuantity);
        for (size_t i = 0; i < static_cast<size_t>(uiData.populationQuantity); ++i)
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
            genomeVec[dim].push_back(std::move(genome));
        }
    }

    return status;
}
