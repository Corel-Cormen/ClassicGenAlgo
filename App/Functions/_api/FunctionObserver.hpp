#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <QStringList>

#include "FunctionInterface.hpp"

class FunctionObserver
{
public:

    explicit FunctionObserver();

    std::vector<QString> getNames() const;

    void choseFunctionId(size_t funcId);

    QString getSelectFuncName();

    std::optional<FunctionType> getSelectType();

private:

    void subscribe(std::unique_ptr<FunctionInterface> func);

    std::vector<std::unique_ptr<FunctionInterface>> functionsVec = {};
    std::optional<size_t> selectFunctionId;
};
