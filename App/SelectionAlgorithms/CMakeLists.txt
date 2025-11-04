add_library(SelectionAlgorithms STATIC
    _api/SelectionAlgoBase.hpp
    _api/BestSelectionAlgo.hpp
    _api/RouletteSelectionAlgo.hpp
    _src/BestSelectionAlgo.cpp
    _src/RouletteSelectionAlgo.cpp
)

target_link_libraries(SelectionAlgorithms
    PUBLIC
    AppTypes
    CommonFunctions
    PRIVATE
    AppTypes
    UiDataHolder
    RandomCore
)

target_include_directories(SelectionAlgorithms
    PUBLIC
    _api
)
