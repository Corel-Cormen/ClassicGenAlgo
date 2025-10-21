add_library(UiDataHolder STATIC
    _api/UiData.h
    _api/UiDataHolderInterface.hpp
    _api/UiDataHolder.hpp
    _src/UiDataHolder.cpp
)

target_link_libraries(UiDataHolder
    PRIVATE
    Qt6::Core
)

target_include_directories(UiDataHolder PUBLIC _api)
