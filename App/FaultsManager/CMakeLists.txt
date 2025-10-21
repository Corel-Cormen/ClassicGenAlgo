add_library(FaultsManager STATIC
    _api/FaultsManagerInterface.hpp
    _api/FaultsManager.hpp
    _src/FaultsManager.cpp
)

target_include_directories(FaultsManager PUBLIC _api)
