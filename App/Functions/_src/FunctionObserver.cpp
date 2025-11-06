#include "FunctionObserver.hpp"
#include "GriewankCEC2014Function.hpp"
#include "HypersphereFunction.hpp"
#include "SchwefelFunction.hpp"
#include "StyblinskiTangFunction.hpp"

FunctionObserver::FunctionObserver()
{
    subscribe(std::make_unique<HypersphereFunction>());
    subscribe(std::make_unique<SchwefelFunction>());
    subscribe(std::make_unique<StyblinskiTangFunction>());
    subscribe(std::make_unique<GriewankCEC2014Function>());
}

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

std::optional<FunctionType> FunctionObserver::getSelectType()
{
    std::optional<FunctionType> result;
    if (selectFunctionId.has_value() &&
        (selectFunctionId.value() < functionsVec.size()))
    {
        result = functionsVec[selectFunctionId.value()].get()->getType();
    }
    return result;
}
