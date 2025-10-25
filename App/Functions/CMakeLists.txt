add_library(Functions STATIC
    _api/FunctionInterface.hpp
    _api/FunctionObserver.hpp
    _api/HypersphereFunction.hpp
    _src/FunctionObserver.cpp
)

target_link_libraries(Functions
    PRIVATE
    Qt6::Core
)

target_include_directories(Functions
    PUBLIC
    _api
)
