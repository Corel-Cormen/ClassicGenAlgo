set(PROJECT_SOURCES
    AppProvider.hpp
    main.cpp
    ${APP_ICON_RESOURCE}
)

qt_add_executable(ClassicGenAlgo
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(ClassicGenAlgo
    PUBLIC
    AppContext
    AppStateMachine
    AppTypes
    FaultsManager
    Functions
    GeneticAlgorithm
    PyModule
    MainDashboard
    UiDataHolder
)

set_target_properties(ClassicGenAlgo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin/ClassicGenAlgo"
)

add_custom_command(TARGET ClassicGenAlgo
    POST_BUILD
    COMMAND Qt6::windeployqt
            --compiler-runtime
            --no-translations
            $<TARGET_FILE:ClassicGenAlgo>
    COMMENT "Running windeployqt..."
)

if (PY_FUNCTION_PATH)
    add_custom_command(TARGET ClassicGenAlgo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PY_FUNCTION_PATH}"
            "$<TARGET_FILE_DIR:ClassicGenAlgo>/_py"
        COMMENT "Copy Python test functions from ${PY_FUNCTION_PATH}"
    )
endif()
