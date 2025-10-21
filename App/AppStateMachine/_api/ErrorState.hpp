#pragma once

#include "StateInterface.hpp"

class FaultsManagerInterface;
class WindowInterface;

class ErrorState : public StateInterface
{
public:
    explicit ErrorState(FaultsManagerInterface &faultsManagerRef,
                        WindowInterface &windowRef);

    void onEntry(QEvent *event) override;

private:

    FaultsManagerInterface &faultsManager;
    WindowInterface &window;

    void run() override;
};
