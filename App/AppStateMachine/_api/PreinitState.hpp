#pragma once

#include "StateInterface.hpp"

class WindowInterface;
class FaultsManagerInterface;

class PreinitState : public StateInterface
{
public:
    explicit PreinitState(FaultsManagerInterface &faultsManagerRef,
                          WindowInterface &windowRef);

    void onEntry(QEvent *event) override;

private:
    FaultsManagerInterface &faultsManager;
    WindowInterface &window;

    void run() override;
};
