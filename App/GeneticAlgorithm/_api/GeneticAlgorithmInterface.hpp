#pragma once

class GeneticAlgorithmInterface
{
public:
    virtual ~GeneticAlgorithmInterface() = default;

    virtual bool setupInitial() = 0;

    virtual void deinit() = 0;

    virtual bool calculate() = 0;
};
