add_library(EvaluateAlgorithms STATIC
    _api/EvaluateAlgoBase.hpp
    _api/PyFunctionEvaluateAlgo.hpp
    _src/PyFunctionEvaluateAlgo.cpp
)

target_link_libraries(EvaluateAlgorithms
    PUBLIC
    AppTypes
    PRIVATE
    Qt6::Core
    AppTypes
    PyModule
)

target_include_directories(EvaluateAlgorithms
    PUBLIC
    _api
)
