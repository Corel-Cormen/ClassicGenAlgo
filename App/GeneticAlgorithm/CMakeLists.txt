add_library(GeneticAlgorithm STATIC
    _api/GeneticAlgorithm.hpp
    _api/GeneticAlgorithmInterface.hpp
    _api/GeneticAlgorithmTypes.hpp
    _api/PopulationFabricBase.hpp
    _api/RandomPopulationFabric.hpp
    _api/EvaluateAlgoBase.hpp
    _api/PyFunctionEvaluateAlgo.hpp
    _api/SelectionAlgoBase.hpp
    _api/BestSelectionAlgo.hpp
    _api/WorstSelectionAlgo.hpp
    _src/GeneticAlgorithm.cpp
    _src/RandomPopulationFabric.cpp
    _src/PyFunctionEvaluateAlgo.cpp
    _src/BestSelectionAlgo.cpp
    _src/WorstSelectionAlgo.cpp
)

target_link_libraries(GeneticAlgorithm
    PUBLIC
    RandomCore
    AppTypes
    CommonFunctions
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
