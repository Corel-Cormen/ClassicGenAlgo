#include <QDebug>

#include "FaultsManagerInterface.hpp"
#include "FunctionObserver.hpp"
#include "GeneticAlgorithm.hpp"
#include "HypersphereFunction.hpp"
#include "UiDataHolderInterface.hpp"

GeneticAlgorithm::GeneticAlgorithm(FaultsManagerInterface &faultsManagerRef,
                                   UiDataHolderInterface &uiDataHolderRef,
                                   FunctionObserver &functionObserverRef) :
    faultsManager{faultsManagerRef},
    uiDataHolder{uiDataHolderRef},
    functionObserver{functionObserverRef}
{}

void GeneticAlgorithm::initProperties()
{
    uiDataHolder.resetDefault();
    functionObserver.subscribe(std::make_unique<HypersphereFunction>());
}

bool GeneticAlgorithm::initPopulation()
{
    bool result = false;
    const auto& uiData = uiDataHolder.getRef();

    random.init(uiData.randomSeed);

    if (createPopulation(uiData))
    {
        qDebug() << "Create population success";
        result = true;
    }
    else
    {
        qDebug() << "Create population error";
        faultsManager.updateFault(Faults::CREATE_POPULATION_ERROR, true);
    }

    return result;
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
