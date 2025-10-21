#pragma once

#include <QObject>
#include <QState>

class StateInterface : public QState
{
    Q_OBJECT

public:
    StateInterface(QState *parent = nullptr)
        : QState(parent)
    {}

    virtual ~StateInterface() = default;

signals:
    void finish();
    void close();
    void errorDetected();

protected:
    virtual void run() = 0;
};
