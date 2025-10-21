add_library(AppContext STATIC
    _api/AppContextInterface.hpp
    _api/AppContext.hpp
    _src/AppContext.cpp
)

target_link_libraries(AppContext
    PUBLIC
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(AppContext
    PUBLIC
    _api
)
