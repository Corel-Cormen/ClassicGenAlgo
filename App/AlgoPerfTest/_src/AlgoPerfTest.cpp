#include <QDebug>
#include <QElapsedTimer>

#include "AlgoPerfTest.hpp"

AlgoPerfTest::AlgoPerfTest(GeneticAlgorithmInterface &geneticAlgoRef) :
    geneticAlgo{geneticAlgoRef}
{}

bool AlgoPerfTest::setupInitial()
{
    return geneticAlgo.setupInitial();
}

void AlgoPerfTest::deinit()
{
    return geneticAlgo.deinit();
}

bool AlgoPerfTest::calculate()
{
    QElapsedTimer timer;
    timer.start();

    bool status = geneticAlgo.calculate();

    qint64 elapsed = timer.elapsed();
    qDebug() << "Calculate perf time:" << elapsed << "ms";

    return status;
}
