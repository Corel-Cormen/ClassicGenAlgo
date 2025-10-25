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
    geneticAlgorithm.initProperties();

    window.load();
    window.show();

    if (window.isActiveWindow())
    {
        emit finish();
    }
    else
    {
        qDebug() << "Load window error";
        faultsManager.updateFault(Faults::LOAD_WINDOW_ERROR, true);
        emit errorDetected();
    }
}
