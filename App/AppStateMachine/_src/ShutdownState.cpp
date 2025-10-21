#include <QDebug>

#include "AppContextInterface.hpp"
#include "ShutdownState.hpp"

ShutdownState::ShutdownState(AppContextInterface &appContextRef) :
    StateInterface(nullptr),
    appContext{appContextRef}
{}

void ShutdownState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Entry to ShutdownState";
    run();
}

void ShutdownState::run()
{
    appContext.quit();
}
