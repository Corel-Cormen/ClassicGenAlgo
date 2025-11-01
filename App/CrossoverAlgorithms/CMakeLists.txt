add_library(CrossoverAlgorithms STATIC
    _api/CrossoverAlgoBase.hpp
    _api/CrossoverIndexBase.hpp
    _api/SinglePointCrossover.hpp
    _src/SinglePointCrossover.cpp
)

target_link_libraries(CrossoverAlgorithms
    PUBLIC
    AppTypes
    PRIVATE
    Qt6::Core
    AppTypes
    UiDataHolder
)

target_include_directories(CrossoverAlgorithms
    PUBLIC
    _api
)
