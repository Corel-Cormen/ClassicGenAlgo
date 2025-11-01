add_library(PopulationFabric STATIC
    _api/PopulationFabricBase.hpp
    _api/RandomPopulationFabric.hpp
    _src/RandomPopulationFabric.cpp
)

target_link_libraries(PopulationFabric
    PUBLIC
    AppTypes
    PRIVATE
    Qt6::Core
    AppTypes
    UiDataHolder
)

target_include_directories(PopulationFabric
    PUBLIC
    _api
)
