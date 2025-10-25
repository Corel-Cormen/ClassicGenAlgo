#pragma once

class GeneticAlgorithmInterface
{
public:
    virtual ~GeneticAlgorithmInterface() = default;

    virtual void initProperties() = 0;

    virtual bool initPopulation() = 0;
};
