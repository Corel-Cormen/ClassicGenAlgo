#include <QApplication>

#include "AppContext.hpp"

AppContext::AppContext(int argc, char *argv[])
    : app{new QApplication(argc, argv)}
{}

int AppContext::run()
{
    return app->exec();
}

void AppContext::quit()
{
    app->quit();
}
