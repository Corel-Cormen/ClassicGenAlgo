#include "FunctionObserver.hpp"

void FunctionObserver::subscribe(std::unique_ptr<FunctionInterface> func)
{
    functionsVec.emplace_back(std::move(func));
}

std::vector<QString> FunctionObserver::getNames() const
{
    std::vector<QString> result;
    result.reserve(functionsVec.size());

    for (auto funcIt = functionsVec.cbegin(); funcIt != functionsVec.cend(); ++funcIt)
    {
        result.push_back(funcIt->get()->getName().toString());
    }

    return result;
}

void FunctionObserver::choseFunctionId(size_t funcId)
{
    selectFunctionId = funcId;
}

QString FunctionObserver::getSelectFuncName()
{
    QString result = "";
    if (selectFunctionId.has_value() &&
        (selectFunctionId.value() < functionsVec.size()))
    {
        result = functionsVec[selectFunctionId.value()].get()->getCreateFuncName().toString();
    }
    return result;
}
