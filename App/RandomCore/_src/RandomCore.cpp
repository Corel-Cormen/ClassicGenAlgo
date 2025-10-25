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
