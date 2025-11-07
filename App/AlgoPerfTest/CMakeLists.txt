add_library(AlgoPerfTest STATIC
    _api/AlgoPerfTest.hpp
    _src/AlgoPerfTest.cpp
)

target_link_libraries(AlgoPerfTest
    PUBLIC
    GeneticAlgorithm
    PRIVATE
    Qt6::Core
)

target_include_directories(AlgoPerfTest
    PUBLIC
    _api
)
