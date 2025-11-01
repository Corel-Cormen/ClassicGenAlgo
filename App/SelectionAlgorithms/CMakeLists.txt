add_library(SelectionAlgorithms STATIC
    _api/SelectionAlgoBase.hpp
    _api/BestSelectionAlgo.hpp
    _api/WorstSelectionAlgo.hpp
    _src/BestSelectionAlgo.cpp
    _src/WorstSelectionAlgo.cpp
)

target_link_libraries(SelectionAlgorithms
    PUBLIC
    AppTypes
    CommonFunctions
    PRIVATE
    Qt6::Core
    AppTypes
    UiDataHolder
)

target_include_directories(SelectionAlgorithms
    PUBLIC
    _api
)
