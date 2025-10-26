#include <QDebug>

#include "AppContextInterface.hpp"
#include "GeneticAlgorithmInterface.hpp"
#include "ShutdownState.hpp"

ShutdownState::ShutdownState(AppContextInterface &appContextRef,
                             GeneticAlgorithmInterface &geneticAlgorithmRef) :
    StateInterface(nullptr),
    appContext{appContextRef},
    geneticAlgorithm{geneticAlgorithmRef}
{}

void ShutdownState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Entry to ShutdownState";
    run();
}

void ShutdownState::run()
{
    geneticAlgorithm.deinit();
    appContext.quit();
}
