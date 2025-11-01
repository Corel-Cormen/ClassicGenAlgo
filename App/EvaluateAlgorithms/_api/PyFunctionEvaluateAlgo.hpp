#pragma once

#include "EvaluateAlgoBase.hpp"

class PyInterface;

class PyFunctionEvaluateAlgo : public EvaluateAlgoBase<PyFunctionEvaluateAlgo>
{
public:
    PyFunctionEvaluateAlgo(PyInterface &pyInterfaceRef);

    bool evaluate_impl(GA::Types::GenomePopulation &genomeVec);

private:
    PyInterface &pyInterface;
};
