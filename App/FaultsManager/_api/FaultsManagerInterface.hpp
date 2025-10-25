#pragma once

#include <vector>

namespace Faults
{
enum FaultId
{
    LOAD_WINDOW_ERROR,
    INPUT_RANDOM_SEED_ERROR,
    INPUT_FUNCTION_PARAMETERS_ERROR,
    INPUT_ALGORITHM_PARAMETRS_ERROR,
    CREATE_POPULATION_ERROR,
    FAULTS_LENGTH
};
}

class FaultsManagerInterface
{
public:
    typedef std::vector<Faults::FaultId> FaultsVec;

    virtual ~FaultsManagerInterface() = default;

    virtual void updateFault(Faults::FaultId fault, bool status) = 0;

    virtual FaultsVec getFaults() = 0;

    virtual bool isNonRecoverable() = 0;

    virtual bool isRecoverable() = 0;

    virtual void clearRecoverableFaults() = 0;
};
