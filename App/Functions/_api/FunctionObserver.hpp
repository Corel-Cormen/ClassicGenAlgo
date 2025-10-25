#pragma once

#include <memory>
#include <vector>

#include <QStringList>

#include "FunctionInterface.hpp"

class FunctionObserver
{
public:

    void subscribe(std::unique_ptr<FunctionInterface> func);

    void choseFunctionId(size_t funcId);

    std::vector<QString> getNames() const;

private:

    std::vector<std::unique_ptr<FunctionInterface>> functionsVec = {};
};
