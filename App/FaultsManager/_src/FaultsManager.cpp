#include "FaultsManager.hpp"

FaultsManager::FaultsManager()
{
    faultsArray[Faults::LOAD_WINDOW_ERROR].isCritical = true;
}

void FaultsManager::updateFault(Faults::FaultId fault, bool status)
{
    if(fault < Faults::FAULTS_LENGTH)
    {
        faultsArray[fault].isActive = status;
    }
}

FaultsManager::FaultsVec FaultsManager::getFaults()
{
    FaultsManager::FaultsVec result;
    result.reserve(Faults::FAULTS_LENGTH);

    for (size_t idx = 0U ; idx < Faults::FAULTS_LENGTH; ++idx)
    {
        if (faultsArray[idx].isActive)
        {
            result.push_back(static_cast<Faults::FaultId>(idx));
        }
    }

    return result;
}

bool FaultsManager::isNonRecoverable()
{
    bool result = false;

    for (size_t idx = 0U; idx < Faults::FAULTS_LENGTH; ++idx)
    {
        if(faultsArray[idx].isCritical && faultsArray[idx].isActive)
        {
            result = true;
            break;
        }
    }

    return result;
}

bool FaultsManager::isRecoverable()
{
    bool result = false;

    for (size_t idx = 0U; idx < Faults::FAULTS_LENGTH; ++idx)
    {
        if(!faultsArray[idx].isCritical && faultsArray[idx].isActive)
        {
            result = true;
            break;
        }
    }

    return result;
}

void FaultsManager::clearRecoverableFaults()
{
    for (size_t idx = 0U; idx < Faults::FAULTS_LENGTH; ++idx)
    {
        if(!faultsArray[idx].isCritical && faultsArray[idx].isActive)
        {
            faultsArray[idx].isActive = false;
        }
    }
}
