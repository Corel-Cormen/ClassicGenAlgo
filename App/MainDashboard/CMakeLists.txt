if(WIN32)
    set(APP_ICON_RESOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/_assets/AppIcon.rc"
        CACHE INTERNAL "Path to Windows application icon resource file (.rc)"
    )
endif()

add_library(MainDashboard STATIC
    _api/WindowInterface.hpp
    _api/MainWindow.hpp
    _src/MainWindow.cpp
    _src/mainwindow.ui
    _assets/AssetsWindowDashboard.qrc
)

target_link_libraries(MainDashboard
    PUBLIC
    Qt6::Widgets
    PRIVATE
    CommonFunctions
    FaultsManager
    Functions
    UiDataHolder
)

target_include_directories(MainDashboard
    PUBLIC
    _api
)
