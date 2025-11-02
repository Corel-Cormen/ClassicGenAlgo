#include "UiDataHolder.hpp"

void UiDataHolder::resetDefault()
{
    uiData.randomSeed = 0U;
    uiData.functionDimension = 3U;
    uiData.minSearchRange = -1.0;
    uiData.maxSearchRange = 1.0;
    uiData.precission = 6U;
    uiData.populationQuantity = 10U;
    uiData.generations = 10U;
    uiData.selectFunctionId = 0U;
    uiData.selctAlgoIndex = 0U;
    uiData.selectAlgoPopulationQuantity = 3;
    uiData.crossoverAlgoIndex = 0U;
    uiData.crossingPropablity = 0.5;
    uiData.eliteStrategyEnable = false;
    uiData.eliteStrategyQuantity = 2;
}
