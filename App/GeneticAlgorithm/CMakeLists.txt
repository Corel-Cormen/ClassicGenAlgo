add_library(GeneticAlgorithm STATIC
    _api/GeneticAlgorithm.hpp
    _api/GeneticAlgorithmInterface.hpp
    _src/GeneticAlgorithm.cpp
)

target_link_libraries(GeneticAlgorithm
    PUBLIC
    RandomCore
    AppTypes
    PRIVATE
    Qt6::Core
    FaultsManager
    Functions
    PyModule
    UiDataHolder
)

target_include_directories(GeneticAlgorithm
    PUBLIC
    _api
)
