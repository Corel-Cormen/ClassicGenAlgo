#include "AppProvider.hpp"

int main(int argc, char *argv[])
{
    AppProvider::init(argc, argv);
    return AppProvider::run();

    // Genome g;
    // bool status = g.initGenom(-10, 10, 6);
    // qDebug() << "status =" << status;
    // qDebug() << "size =" << g.size();
    // for(size_t i = 0; i < g.size(); ++i)
    // {
    //     g[i] = 1;
    // }
    // qDebug() << "val =" << g.val();
}
