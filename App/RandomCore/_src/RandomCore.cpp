#include <QDateTime>
#include <QDebug>

#include "RandomCore.hpp"

void RandomCore::init(quint32 seed)
{
    if (seed == 0U)
    {
        quint32 dataTimeSeed = static_cast<quint32>(QDateTime::currentMSecsSinceEpoch());
        qDebug() << "Init random engine by time:" << dataTimeSeed;
        generator.seed(dataTimeSeed);
    }
    else
    {
        qDebug() << "Init random engine by provided seed:" << seed;
        generator.seed(seed);
    }
}

size_t RandomCore::randDistIndex(std::vector<qreal> dist) const
{
    size_t randIdx = 0U;
    qreal sumDist = 0.0;
    for (const qreal d : dist)
    {
        sumDist += d;
    }

    const qreal r = randUnit<qreal>();

    qreal cumulative = 0.0;
    for (size_t i = 0; i < dist.size(); ++i)
    {
        cumulative += dist[i] / sumDist;
        if (r < cumulative)
        {
            randIdx = i;
            break;
        }
    }

    return randIdx;
}
