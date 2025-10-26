#pragma once

#include "StateInterface.hpp"

class AppContextInterface;
class GeneticAlgorithmInterface;

class ShutdownState : public StateInterface
{
public:
    explicit ShutdownState(AppContextInterface &appContextRef,
                           GeneticAlgorithmInterface &geneticAlgorithmRef);

    void onEntry(QEvent *event) override;

private:
    AppContextInterface &appContext;
    GeneticAlgorithmInterface &geneticAlgorithm;

    void run() override;
};
