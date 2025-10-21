#pragma once

#include "StateInterface.hpp"

class FaultsManagerInterface;
class WindowInterface;

class InitRunState : public StateInterface
{
public:
    explicit InitRunState(FaultsManagerInterface &faultsManagerRef,
                          WindowInterface &windowRef);

    void onEntry(QEvent *event) override;

private:
    FaultsManagerInterface &faultsManager;
    WindowInterface &window;

    void run() override;

private slots:
    void onWindowClosed();
    void onTriggerCalculate();
};
