#include <QStateMachine>

#include "StateInterface.hpp"
#include "StateMachine.hpp"

StateMachine::StateMachine(StateInterface &preinitState,
                           StateInterface &initrunState,
                           StateInterface &runState,
                           StateInterface &errorState,
                           StateInterface &shutdownState)
    : QObject(nullptr),
    machine{new QStateMachine(this)},
    preinitState{preinitState},
    initrunState{initrunState},
    runState{runState},
    errorState{errorState},
    shutdownState{shutdownState}
{
    machine->addState(&preinitState);
    machine->addState(&initrunState);
    machine->addState(&runState);
    machine->addState(&errorState);
    machine->addState(&shutdownState);

    preinitState.addTransition(&preinitState, &StateInterface::finish, &initrunState);
    preinitState.addTransition(&preinitState, &StateInterface::errorDetected, &errorState);

    initrunState.addTransition(&initrunState, &StateInterface::finish, &runState);
    initrunState.addTransition(&initrunState, &StateInterface::errorDetected, &errorState);
    initrunState.addTransition(&initrunState, &StateInterface::close, &shutdownState);

    runState.addTransition(&runState, &StateInterface::finish, &initrunState);
    runState.addTransition(&runState, &StateInterface::errorDetected, &errorState);

    errorState.addTransition(&errorState, &StateInterface::finish, &initrunState);
    errorState.addTransition(&errorState, &StateInterface::close, &shutdownState);

    machine->setInitialState(&preinitState);
    machine->start();
}

StateMachine::~StateMachine()
{
    machine->removeState(&preinitState);
    machine->removeState(&initrunState);
    machine->removeState(&runState);
    machine->removeState(&errorState);
    machine->removeState(&shutdownState);
}
