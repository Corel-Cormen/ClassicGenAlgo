add_library(CommonFunctions STATIC
    _api/CommonFunctions.hpp
)

target_include_directories(CommonFunctions
    PUBLIC
    _api
)
