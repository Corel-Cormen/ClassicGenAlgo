#ifndef UI_DATA_H
#define UI_DATA_H

#include <QtTypes>

struct UiData
{
    quint32 randomSeed;
    qreal minSearchRange;
    qreal maxSearchRange;
    quint8 precission;
    quint16 populationQuantity;
    quint16 generations;
};

#endif // UI_DATA_H
