#include "FunctionObserver.hpp"
#include "HypersphereFunction.hpp"
#include "HyperellipsoidFunction.hpp"
#include "RosenbrockFunction.hpp"
#include "RastriginFunction.hpp"
#include "SchwefelFunction.hpp"
#include "GriewankFunction.hpp"
#include "AckleyFunction.hpp"
#include "MichalewiczFunction.hpp"
#include "EggHolderFunction.hpp"
#include "KeaneFunction.hpp"
#include "RanaFunction.hpp"
#include "EasomFunction.hpp"
#include "DeJong3Function.hpp"
#include "DeJong5Function.hpp"
#include "GoldsteinAndPrice.hpp"
#include "PichenyGoldsteinAndPrice.hpp"
#include "StyblinskiTangFunction.hpp"
#include "McCormickFunction.hpp"
#include "Schaffer2Function.hpp"
#include "HimmelblauFunction.hpp"
#include "PitsAndHolesFunction.hpp"
#include "BC_1.hpp"
#include "BC_2.hpp"
#include "BC_35.hpp"
#include "GriewankCEC2014Function.hpp"

FunctionObserver::FunctionObserver()
{
    subscribe(std::make_unique<HypersphereFunction>());
    subscribe(std::make_unique<HyperellipsoidFunction>());
    subscribe(std::make_unique<RosenbrockFunction>());
    subscribe(std::make_unique<RastriginFunction>());
    subscribe(std::make_unique<SchwefelFunction>());
    subscribe(std::make_unique<GriewankFunction>());
    subscribe(std::make_unique<AckleyFunction>());
    subscribe(std::make_unique<MichalewiczFunction>());
    subscribe(std::make_unique<EggHolderFunction>());
    subscribe(std::make_unique<KeaneFunction>());
    subscribe(std::make_unique<RanaFunction>());
    subscribe(std::make_unique<EasomFunction>());
    subscribe(std::make_unique<DeJong3Function>());
    subscribe(std::make_unique<DeJong5Function>());
    subscribe(std::make_unique<GoldsteinAndPrice>());
    subscribe(std::make_unique<PichenyGoldsteinAndPrice>());
    subscribe(std::make_unique<StyblinskiTangFunction>());
    subscribe(std::make_unique<McCormickFunction>());
    subscribe(std::make_unique<Schaffer2Function>());
    subscribe(std::make_unique<HimmelblauFunction>());
    subscribe(std::make_unique<PitsAndHolesFunction>());
    subscribe(std::make_unique<BC_1>());
    subscribe(std::make_unique<BC_2>());
    subscribe(std::make_unique<BC_35>());
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
