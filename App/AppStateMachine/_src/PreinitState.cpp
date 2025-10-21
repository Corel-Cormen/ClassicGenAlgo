#include <QDebug>

#include "FaultsManagerInterface.hpp"
#include "WindowInterface.hpp"
#include "PreinitState.hpp"

PreinitState::PreinitState(FaultsManagerInterface &faultsManagerRef,
                           WindowInterface &windowRef) :
    StateInterface(nullptr),
    window{windowRef},
    faultsManager{faultsManagerRef}
{}

void PreinitState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Entry to InitState";
    run();
}

void PreinitState::run()
{
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
