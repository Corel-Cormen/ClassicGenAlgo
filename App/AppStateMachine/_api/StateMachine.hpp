#pragma once

#include <QObject>

class QStateMachine;
class StateInterface;

class StateMachine : public QObject
{
    Q_OBJECT

public:
    explicit StateMachine(StateInterface &initState,
                          StateInterface &initrunState,
                          StateInterface &runState,
                          StateInterface &errorState,
                          StateInterface &shutdownState);

    ~StateMachine();

private:
    QStateMachine *machine;
    StateInterface &preinitState;
    StateInterface &initrunState;
    StateInterface &runState;
    StateInterface &errorState;
    StateInterface &shutdownState;
};
