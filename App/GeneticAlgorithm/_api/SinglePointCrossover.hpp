#pragma once

#include "CrossoverAlgoBase.hpp"

#include <iostream>

class SinglePointCrossover : public CrossoverAlgoBase<SinglePointCrossover>
{
public:
    bool crossover_impl(const GA::Types::GenomePoint &g1,
                        const GA::Types::GenomePoint &g2,
                        GA::Types::GenomePoint &out)
    {
        // for (size_t i = 0; i < g1.point.size(); i++)
        // {
        //     // std::cout << "g1[" << i << "] = ";
        //     // for (size_t j = 0; j < g1.point[i].size(); ++j)
        //     // {
        //     //     std::cout << g1.point[i][j];
        //     // }
        //     // std::cout << std::endl;

        //     // std::cout << "g2[" << i << "] = ";
        //     // for (size_t j = 0; j < g2.point[i].size(); ++j)
        //     // {
        //     //     std::cout << g2.point[i][j];
        //     // }
        //     // std::cout << std::endl;
        // }
        return true;
    }
};
