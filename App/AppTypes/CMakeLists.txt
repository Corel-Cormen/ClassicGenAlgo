add_library(AppTypes STATIC
    _api/GeneticAlgorithmTypes.hpp
    _api/Genome.hpp
    _src/Genome.cpp
)

target_link_libraries(AppTypes
    PRIVATE
    Qt6::Core
    CommonFunctions
    RandomCore
)

target_include_directories(AppTypes
    PUBLIC
    _api
)
