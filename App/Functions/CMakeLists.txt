set(PY_FUNCTION_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/_py"
    CACHE INTERNAL "Path to Python test functions"
)

add_library(Functions STATIC
    _api/FunctionInterface.hpp
    _api/FunctionObserver.hpp
    _src/FunctionObserver.cpp
    _src/HypersphereFunction.hpp
    _src/HyperellipsoidFunction.hpp
    _src/RosenbrockFunction.hpp
    _src/RastriginFunction.hpp
    _src/SchwefelFunction.hpp
    _src/GriewankFunction.hpp
    _src/AckleyFunction.hpp
    _src/MichalewiczFunction.hpp
    _src/EggHolderFunction.hpp
    _src/KeaneFunction.hpp
    _src/RanaFunction.hpp
    _src/EasomFunction.hpp
    _src/DeJong3Function.hpp
    _src/DeJong5Function.hpp
    _src/GoldsteinAndPrice.hpp
    _src/PichenyGoldsteinAndPrice.hpp
    _src/StyblinskiTangFunction.hpp
    _src/McCormickFunction.hpp
    _src/Schaffer2Function.hpp
    _src/HimmelblauFunction.hpp
    _src/PitsAndHolesFunction.hpp
    _src/BC_1.hpp
    _src/BC_2.hpp
    _src/BC_35.hpp
    _src/GriewankCEC2014Function.hpp
)

target_link_libraries(Functions
    PRIVATE
    Qt6::Core
)

target_include_directories(Functions
    PUBLIC
    _api
)
