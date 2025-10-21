#include "UiDataHolder.hpp"

void UiDataHolder::resetDefault()
{
    uiData.randomSeed = 0U;
    uiData.minSearchRange = -1.0;
    uiData.maxSearchRange = 1.0;
    uiData.precission = 5U;
    uiData.populationQuantity = 10U;
    uiData.generations = 10U;
}
