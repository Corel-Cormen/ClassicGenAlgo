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

} // end Types
} // end GA
