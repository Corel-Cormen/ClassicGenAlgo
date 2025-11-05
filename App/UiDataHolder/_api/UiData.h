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
    quint16 populationQuantity;
    quint16 generations;
    quint32 randomSeed;
    qreal minSearchRange;
    qreal maxSearchRange;
    std::vector<quint8> selectAlgoNames;
    quint8 selctAlgoIndex;
    quint16 selectAlgoPopulationQuantity;
    quint8 tournamentPopulationQuantity;
    std::vector<quint8> crossoverAlgoNames;
    quint8 crossoverAlgoIndex;
    qreal crossingPropablity;
    bool eliteStrategyEnable;
    quint16 eliteStrategyQuantity;
};

#endif // UI_DATA_H
