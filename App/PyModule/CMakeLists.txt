find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

add_library(PyModule STATIC
    _api/PyInterface.hpp
    _api/PyQt.hpp
    _src/PyQt.cpp
)

target_link_libraries(PyModule
    PRIVATE
    Qt6::Core
    ${Python3_LIBRARIES}
)

target_include_directories(PyModule
    PUBLIC
    _api
    PRIVATE
    ${Python3_INCLUDE_DIRS}
)
