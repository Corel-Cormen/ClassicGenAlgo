#include <QDebug>

#include "GeneticAlgorithmInterface.hpp"
#include "RunState.hpp"

RunState::RunState(GeneticAlgorithmInterface &geneticAlgorithmRef) :
    StateInterface(nullptr),
    geneticAlgorithm{geneticAlgorithmRef}
{}

void RunState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Entry to RunState";
    run();
}

void RunState::run()
{
    bool status = geneticAlgorithm.initPopulation();

    if (status)
    {
        qDebug() << "Algorithm done with success!";
        emit finish();
    }
    else
    {
        qDebug() << "Execution genetic algorithm error";
        emit errorDetected();
    }
}
