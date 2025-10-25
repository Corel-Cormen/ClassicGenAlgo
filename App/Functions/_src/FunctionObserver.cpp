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
        result.push_back(funcIt->get()->name().toString());
    }

    return result;
}
