add_library(CrossoverAlgorithms STATIC
    _api/CrossoverAlgoBase.hpp
    _api/CrossoverIndexBase.hpp
    _api/SinglePointCrossover.hpp
    _api/TwoPointCrossover.hpp
    _api/UniformCrossover.hpp
    _api/DiscreteCrossover.hpp
    _api/ArithmeticCrossover.hpp
    _api/AveragingCrossover.hpp
    _api/BlendAlphaCrossover.hpp
    _api/BlendAlphaBetaCrossover.hpp
    _api/LinearCrossover.hpp
    _src/SinglePointCrossover.cpp
    _src/TwoPointCrossover.cpp
    _src/UniformCrossover.cpp
    _src/DiscreteCrossover.cpp
    _src/ArithmeticCrossover.cpp
    _src/AveragingCrossover.cpp
    _src/BlendAlphaCrossover.cpp
    _src/BlendAlphaBetaCrossover.cpp
    _src/LinearCrossover.cpp
)

target_link_libraries(CrossoverAlgorithms
    PUBLIC
    AppTypes
    PRIVATE
    AppTypes
    CommonFunctions
    EvaluateAlgorithms
    RandomCore
    UiDataHolder
)

target_include_directories(CrossoverAlgorithms
    PUBLIC
    _api
)
