#pragma once

#include "StateInterface.hpp"

class GeneticAlgorithmInterface;

class RunState : public StateInterface
{
public:
    explicit RunState(GeneticAlgorithmInterface &geneticAlgorithmRef);

    void onEntry(QEvent *event) override;

private:
    GeneticAlgorithmInterface &geneticAlgorithm;

    void run() override;
};
