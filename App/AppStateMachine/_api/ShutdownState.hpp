#pragma once

#include "StateInterface.hpp"

class AppContextInterface;

class ShutdownState : public StateInterface
{
public:
    explicit ShutdownState(AppContextInterface &appContextRef);

    void onEntry(QEvent *event) override;

private:
    AppContextInterface &appContext;

    void run() override;
};
