add_library(GeneticAlgorithm STATIC
    _api/GeneticAlgorithm.hpp
    _api/GeneticAlgorithmInterface.hpp
    _src/GeneticAlgorithm.cpp
)

target_link_libraries(GeneticAlgorithm
    PUBLIC
    RandomCore
    PopulationFabric
    EvaluateAlgorithms
    SelectionAlgorithms
    CrossoverAlgorithms
    MutationAlgorithms
    PRIVATE
    Qt6::Core
    FaultsManager
    Functions
    PyModule
    UiDataHolder
    AlgoLogger
)

target_include_directories(GeneticAlgorithm
    PUBLIC
    _api
)
