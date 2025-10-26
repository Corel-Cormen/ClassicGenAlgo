#include <QDebug>

#include "FaultsManagerInterface.hpp"
#include "GeneticAlgorithmInterface.hpp"
#include "WindowInterface.hpp"
#include "PreinitState.hpp"

PreinitState::PreinitState(FaultsManagerInterface &faultsManagerRef,
                           WindowInterface &windowRef,
                           GeneticAlgorithmInterface &geneticAlgorithmRef) :
    StateInterface(nullptr),
    window{windowRef},
    faultsManager{faultsManagerRef},
    geneticAlgorithm{geneticAlgorithmRef}
{}

void PreinitState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Entry to InitState";
    run();
}

void PreinitState::run()
{
    if (geneticAlgorithm.setupInitial())
    {
        window.load();
    }

    if (!faultsManager.isNonRecoverable())
    {
        emit finish();
    }
    else
    {
        emit errorDetected();
    }
}
