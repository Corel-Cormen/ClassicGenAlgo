add_library(AlgoLogger STATIC
    _api/AlgoLoggerInterface.hpp
    _api/AlgoStatisticLogger.hpp
    _src/AlgoStatisticLogger.cpp
)

target_link_libraries(AlgoLogger
    PUBLIC
    AppTypes
    PRIVATE
    Qt6::Core
    CommonFunctions
)

target_include_directories(AlgoLogger
    PUBLIC
    _api
)
