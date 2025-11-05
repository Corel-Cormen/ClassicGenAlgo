#ifndef UI_DATA_H
#define UI_DATA_H

#include <vector>

#include <QtTypes>
#include <QStringView>

struct UiData
{
    quint8 functionDimension;
    quint8 precission;
    quint8 selectFunctionId;
    quint32 populationQuantity;
    quint16 generations;
    quint32 randomSeed;
    qreal minSearchRange;
    qreal maxSearchRange;
    std::vector<quint8> selectAlgoNames;
    quint8 selctAlgoIndex;
    quint32 selectAlgoPopulationQuantity;
    quint8 tournamentPopulationQuantity;
    std::vector<quint8> crossoverAlgoNames;
    quint8 crossoverAlgoIndex;
    qreal crossingPropablity;
    std::vector<quint8> mutationAlgoNames;
    quint8 mutationAlgoIndex;
    qreal mutationPropablity;
    bool eliteStrategyEnable;
    quint32 eliteStrategyQuantity;
};

#endif // UI_DATA_H
