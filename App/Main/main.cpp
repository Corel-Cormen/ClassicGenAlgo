#include "AppProvider.hpp"

int main(int argc, char *argv[])
{
    AppProvider::init(argc, argv);
    return AppProvider::run();
}
