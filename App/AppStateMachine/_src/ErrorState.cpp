#include <QDebug>

#include "ErrorState.hpp"
#include "FaultsManager.hpp"
#include "WindowInterface.hpp"

ErrorState::ErrorState(FaultsManagerInterface &faultsManagerRef,
                       WindowInterface &windowRef) :
    StateInterface(nullptr),
    faultsManager{faultsManagerRef},
    window{windowRef}
{}

void ErrorState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Entry to ErrorState";

    run();
}

void ErrorState::run()
{
    for (const Faults::FaultId fault : faultsManager.getFaults())
    {
        switch (fault)
        {
        case Faults::LOAD_WINDOW_ERROR:
            window.showCriticalMessage("Load window critical error.");
            break;
        case Faults::INPUT_RANDOM_SEED_ERROR:
            window.showWarningMessage("Verify input random seed.");
            break;
        case Faults::INPUT_FUNCTION_PARAMETERS_ERROR:
            window.showWarningMessage("Verify input function parameters.");
            break;
        case Faults::INPUT_ALGORITHM_PARAMETRS_ERROR:
            window.showWarningMessage("Verify input algorithm parameters.");
            break;
        case Faults::GENETIC_ALGORITHM_ERROR:
            window.showWarningMessage("Verify input algorithm parameters.\nError algorith during calculate.");
            break;
        case Faults::RUN_PYTHON_ERROR:
        case Faults::PYTHON_FUNC_CALLER_ERROR:
            window.showCriticalMessage("Run Python error.");
            break;
        default:
            qDebug() << "Unknow fault id:" << fault;
            break;
        }
    }

    if (faultsManager.isNonRecoverable())
    {
        emit close();
    }
    else if (faultsManager.isRecoverable())
    {
        faultsManager.clearRecoverableFaults();
        emit finish();
    }
    else
    {
        // do nothing
    }
}
