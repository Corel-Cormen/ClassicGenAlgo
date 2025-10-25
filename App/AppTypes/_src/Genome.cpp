#include "Genome.hpp"
#include "CommonFunctions.hpp"
#include "RandomCore.hpp"

bool Genome::createGenom(const qreal _beginSearch,
                         const qreal _endSearch,
                         const size_t _precission)
{
    bool result = false;
    if (CommonFunc::lessEqThan(_beginSearch, _endSearch))
    {
        constexpr size_t maxLength = 128U;
        size_t lenbit = static_cast<size_t>(std::ceil(
            std::log2((_endSearch - _beginSearch) *
            std::pow(10.0, _precission) + std::log2(1.0))));

        if(lenbit <= maxLength)
        {
            genType.resize(lenbit);
            beginSearch = _beginSearch;
            endSearch = _endSearch;
            result = true;
        }
    }
    return result;
}

void Genome::initGenom(const RandomCore& rand)
{
    for(size_t idx = 0; idx < genType.size(); ++idx)
    {
        genType[idx] = rand.randUnit<bool>();
    }
}

qreal Genome::val() const
{
    quint128 raw = 0;
    for (size_t idx = 0; idx < genType.size(); ++idx)
    {
        raw <<= 1U;
        raw |= static_cast<quint128>(genType[idx] ? 1U : 0U);
    }

    qreal result = beginSearch + raw *
        ((endSearch - beginSearch) / (std::pow(2.0, genType.size()) - 1.0));

    return result;
}
