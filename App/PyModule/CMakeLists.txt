add_library(PyModule STATIC
    _api/PyInterface.hpp
    _api/PyQt.hpp
    _src/PyQt.cpp
)

get_target_property(PY_PROGRAM_ROOT PyModule RUNTIME_OUTPUT_DIRECTORY)
set(PY_PROGRAM_ROOT "${PY_PROGRAM_ROOT}/python")
execute_process(
    COMMAND powershell -ExecutionPolicy Bypass -File
        "${CMAKE_SOURCE_DIR}/scripts/build_python_install.ps1"
        -PyPath "${PY_PROGRAM_ROOT}"
        -PyVersion "${PY_VERSION}"
        -PyPlatform "${PY_PLATFORM}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE PY_SETUP_RESULT
)
if(NOT PY_SETUP_RESULT EQUAL 0)
    message(FATAL_ERROR "Setup Python script failed (code: ${result})")
endif()

set(PY_INCLUDE_DIR "${PY_PROGRAM_ROOT}/include")
set(PY_LIB_DIR "${PY_PROGRAM_ROOT}/libs")

file(GLOB PY_LIBRARIES "${PY_LIB_DIR}/*.lib" "${PY_PROGRAM_ROOT}/*.dll")

set(Python3_INCLUDE_DIRS "${PY_INCLUDE_DIR}" CACHE PATH "Python include dir")
set(Python3_LIBRARIES "${PY_LIBRARIES}" CACHE FILEPATH "Python library")

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
