add_library(RandomCore STATIC
    _api/RandomCore.hpp
    _src/RandomCore.cpp
)

target_link_libraries(RandomCore
    PRIVATE
    Qt6::Core
)

target_include_directories(RandomCore
    PUBLIC
    _api
)
