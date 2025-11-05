add_library(MutationAlgorithms STATIC
    _api/MutationAlgoBase.hpp
    _api/EdgeMutationAlgo.hpp
    _api/OnePointMutationAlgo.hpp
    _api/TwoPointMutationAlgo.hpp
    _api/InwerseMutationAlgo.hpp
    _src/EdgeMutationAlgo.cpp
    _src/OnePointMutationAlgo.cpp
    _src/TwoPointMutationAlgo.cpp
    _src/InwerseMutationAlgo.cpp
)

target_link_libraries(MutationAlgorithms
    PUBLIC
    AppTypes
    PRIVATE
    UiDataHolder
    RandomCore
)

target_include_directories(MutationAlgorithms
    PUBLIC
    _api
)
