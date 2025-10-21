#pragma once

#include "AppContextInterface.hpp"

class QApplication;

class AppContext : public AppContextInterface
{
public:
    explicit AppContext(int argc, char *argv[]);

    int run() override;
    void quit() override;

private:
    QApplication *app;
};
