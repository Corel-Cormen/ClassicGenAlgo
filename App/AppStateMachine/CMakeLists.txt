add_library(AppStateMachine STATIC
    _api/StateInterface.hpp
    _api/StateMachine.hpp
    _api/PreinitState.hpp
    _api/InitRunState.hpp
    _api/ErrorState.hpp
    _api/RunState.hpp
    _api/ShutdownState.hpp
    _src/StateMachine.cpp
    _src/PreinitState.cpp
    _src/InitRunState.cpp
    _src/ErrorState.cpp
    _src/RunState.cpp
    _src/ShutdownState.cpp
)

target_link_libraries(AppStateMachine
    PUBLIC
    Qt6::StateMachine
    PRIVATE
    AppContext
    FaultsManager
    GeneticAlgorithm
    MainDashboard
)

target_include_directories(AppStateMachine PUBLIC _api)
