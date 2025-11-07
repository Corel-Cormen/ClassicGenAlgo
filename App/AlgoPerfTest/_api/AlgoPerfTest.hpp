#pragma once

#include "GeneticAlgorithmInterface.hpp"

class AlgoPerfTest : public GeneticAlgorithmInterface
{
public:
    AlgoPerfTest(GeneticAlgorithmInterface &geneticAlgoRef);

    bool setupInitial() override;

    void deinit() override;

    bool calculate() override;

private:
    GeneticAlgorithmInterface &geneticAlgo;
};
