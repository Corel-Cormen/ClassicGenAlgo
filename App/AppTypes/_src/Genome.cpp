#include "Genome.hpp"
#include "CommonFunctions.hpp"
#include "RandomCore.hpp"

Genome::Genome(GenomeValType genomeValType) :
    genomeValType{genomeValType}
{}

Genome::Genome(const Genome& other)
    : genomeValType(other.genomeValType),
    genType(other.genType),
    beginSearch(other.beginSearch),
    endSearch(other.endSearch),
    realValue(other.realValue)
{}

Genome::Genome(Genome&& other) noexcept
    : genomeValType(other.genomeValType),
    genType(std::move(other.genType)),
    beginSearch(other.beginSearch),
    endSearch(other.endSearch),
    realValue(other.realValue)
{}

Genome& Genome::operator=(const Genome& other)
{
    if (this != &other)
    {
        genType      = other.genType;
        beginSearch  = other.beginSearch;
        endSearch    = other.endSearch;
        realValue    = other.realValue;
    }
    return *this;
}

Genome& Genome::operator=(Genome&& other) noexcept
{
    if (this != &other)
    {
        genType      = std::move(other.genType);
        beginSearch  = other.beginSearch;
        endSearch    = other.endSearch;
        realValue    = other.realValue;
    }
    return *this;
}

bool Genome::createGenom(const qreal _beginSearch,
                         const qreal _endSearch,
                         const size_t _precission)
{
    bool result = false;

    if(genomeValType == GenomeValType::BINARY_GENOME_TYPE)
    {
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
    }
    else
    {
        genType.resize(0);
        beginSearch = _beginSearch;
        endSearch = _endSearch;
        result = true;
    }

    return result;
}

void Genome::initGenom(const RandomCore& rand)
{
    if(genomeValType == GenomeValType::BINARY_GENOME_TYPE)
    {
        for(size_t idx = 0; idx < genType.size(); ++idx)
        {
            genType[idx] = rand.randUnit<bool>();
        }
    }
    else
    {
        realValue = rand.randNumber(beginSearch, endSearch);
    }
}

qreal Genome::val() const
{
    if(genomeValType == GenomeValType::BINARY_GENOME_TYPE)
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
    else
    {
        return realValue;
    }
}
