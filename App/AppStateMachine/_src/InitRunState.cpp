#include <QDebug>

#include "FaultsManagerInterface.hpp"
#include "InitRunState.hpp"
#include "WindowInterface.hpp"

InitRunState::InitRunState(FaultsManagerInterface &faultsManagerRef,
                           WindowInterface &windowRef) :
    StateInterface(nullptr),
    faultsManager{faultsManagerRef},
    window{windowRef}
{}

void InitRunState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Entry to InitRunState";

    connect(&window, &WindowInterface::windowClosed,
            this, &InitRunState::onWindowClosed);
    connect(&window, &WindowInterface::triggerCalculate,
            this, &InitRunState::onTriggerCalculate);

    run();
}

void InitRunState::run()
{}

void InitRunState::onWindowClosed()
{
    emit close();
}

void InitRunState::onTriggerCalculate()
{
    if (!faultsManager.isNonRecoverable() &&
        !faultsManager.isRecoverable())
    {
        emit finish();
    }
    else
    {
        emit errorDetected();
    }
}
