#pragma once

#include <vector>

#include <QtTypes>

#include "Genome.hpp"

namespace GA
{
namespace Types
{

struct GenomePoint
{
    std::vector<Genome> point;
    qreal value;
};

using GenomePopulation = std::vector<GenomePoint>;

struct Points
{
    std::vector<qreal> point;
    qreal value;
};

} // end Types
} // end GA
