#include "PyFunctionEvaluateAlgo.hpp"
#include "PyInterface.hpp"

PyFunctionEvaluateAlgo::PyFunctionEvaluateAlgo(PyInterface &pyInterfaceRef) :
    pyInterface{pyInterfaceRef}
{}

bool PyFunctionEvaluateAlgo::evaluate_impl(GA::Types::GenomePopulation &genomeVec)
{
    bool calcStatus = false;

    for (auto& genomAxisPoint : genomeVec)
    {
        std::vector<qreal> point;
        for (auto& genomPoint : genomAxisPoint.point)
        {
            point.push_back(genomPoint.val());
        }

        calcStatus = pyInterface.calcPoint(point, genomAxisPoint.value);
        if (!calcStatus)
        {
            break;
        }
    }

    return calcStatus;
}
