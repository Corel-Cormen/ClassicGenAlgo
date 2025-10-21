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
