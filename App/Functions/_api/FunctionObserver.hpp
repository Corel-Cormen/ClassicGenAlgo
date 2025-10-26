#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <QStringList>

#include "FunctionInterface.hpp"

class FunctionObserver
{
public:

    void subscribe(std::unique_ptr<FunctionInterface> func);

    std::vector<QString> getNames() const;

    void choseFunctionId(size_t funcId);

    QString getSelectFuncName();

private:

    std::vector<std::unique_ptr<FunctionInterface>> functionsVec = {};
    std::optional<size_t> selectFunctionId;
};
