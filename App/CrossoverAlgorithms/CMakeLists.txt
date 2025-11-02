add_library(CrossoverAlgorithms STATIC
    _api/CrossoverAlgoBase.hpp
    _api/CrossoverIndexBase.hpp
    _api/SinglePointCrossover.hpp
    _api/TwoPointCrossover.hpp
    _api/UniformCrossover.hpp
    _api/DiscreteCrossover.hpp
    _src/SinglePointCrossover.cpp
    _src/TwoPointCrossover.cpp
    _src/UniformCrossover.cpp
    _src/DiscreteCrossover.cpp
)

target_link_libraries(CrossoverAlgorithms
    PUBLIC
    AppTypes
    PRIVATE
    AppTypes
    CommonFunctions
    RandomCore
    UiDataHolder
)

target_include_directories(CrossoverAlgorithms
    PUBLIC
    _api
)
