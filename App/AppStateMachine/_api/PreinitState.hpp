#pragma once

#include "StateInterface.hpp"

class WindowInterface;
class FaultsManagerInterface;
class GeneticAlgorithmInterface;

class PreinitState : public StateInterface
{
public:
    explicit PreinitState(FaultsManagerInterface &faultsManagerRef,
                          WindowInterface &windowRef,
                          GeneticAlgorithmInterface &geneticAlgorithmRef);

    void onEntry(QEvent *event) override;

private:
    FaultsManagerInterface &faultsManager;
    WindowInterface &window;
    GeneticAlgorithmInterface &geneticAlgorithm;

    void run() override;
};
