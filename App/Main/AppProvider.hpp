#pragma once

#include <memory>

#include "AlgoStatisticLogger.hpp"
#include "AlgoPerfTest.hpp"
#include "AppContext.hpp"
#include "ErrorState.hpp"
#include "FaultsManager.hpp"
#include "FunctionObserver.hpp"
#include "GeneticAlgorithm.hpp"
#include "InitRunState.hpp"
#include "MainWindow.hpp"
#include "PreinitState.hpp"
#include "PyQt.hpp"
#include "RunState.hpp"
#include "ShutdownState.hpp"
#include "StateMachine.hpp"
#include "UiDataHolder.hpp"

class AppProvider
{
public:
    static void init(int argc, char *argv[])
    {
        context = std::make_unique<AppContext>(argc, argv);
        uiDataHolder = std::make_unique<UiDataHolder>();
        faultsManager = std::make_unique<FaultsManager>();
        functionsObserver = std::make_unique<FunctionObserver>();
        python = std::make_unique<PyQt>();
        algoLogger = std::make_unique<AlgoStatisticLogger>();

        window = std::make_unique<MainWindow>(*faultsManager, *uiDataHolder, *functionsObserver);
        geneticAlgorithm = std::make_unique<GeneticAlgorithm>(*faultsManager,
                                                              *uiDataHolder,
                                                              *functionsObserver,
                                                              *python,
                                                              *algoLogger);

        algoPerfTest = std::make_unique<AlgoPerfTest>(*geneticAlgorithm);

        preinitState = std::make_unique<PreinitState>(*faultsManager, *window, *geneticAlgorithm);
        initrunState = std::make_unique<InitRunState>(*faultsManager, *window);
        runState = std::make_unique<RunState>(*algoPerfTest);
        errorState = std::make_unique<ErrorState>(*faultsManager, *window);
        shutdownState = std::make_unique<ShutdownState>(*context, *geneticAlgorithm);
        machine = std::make_unique<StateMachine>(*preinitState, *initrunState, *runState, *errorState, *shutdownState);
    }

    static int run()
    {
        return context->run();
    }

private:
    inline static std::unique_ptr<AppContextInterface> context;
    inline static std::unique_ptr<UiDataHolderInterface> uiDataHolder;
    inline static std::unique_ptr<FaultsManagerInterface> faultsManager;
    inline static std::unique_ptr<FunctionObserver> functionsObserver;
    inline static std::unique_ptr<PyInterface> python;
    inline static std::unique_ptr<AlgoLoggerInterface> algoLogger;
    inline static std::unique_ptr<AlgoPerfTest> algoPerfTest;

    inline static std::unique_ptr<WindowInterface> window;
    inline static std::unique_ptr<GeneticAlgorithmInterface> geneticAlgorithm;

    inline static std::unique_ptr<StateInterface> preinitState;
    inline static std::unique_ptr<StateInterface> initrunState;
    inline static std::unique_ptr<StateInterface> runState;
    inline static std::unique_ptr<StateInterface> errorState;
    inline static std::unique_ptr<StateInterface> shutdownState;
    inline static std::unique_ptr<StateMachine> machine;
};
