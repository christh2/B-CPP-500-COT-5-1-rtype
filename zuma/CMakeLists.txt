cmake_minimum_required(VERSION 3.10)
project(R-Type VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DPLATFORM_LINUX)
elseif(APPLE)
    add_definitions(-DPLATFORM_MAC)
endif()

set(ENGINE
    ../nova_engine/src/physics_system/collide_detection_system.cpp
    ../nova_engine/src/physics_system/collision_apply_system.cpp
    ../nova_engine/src/physics_system/gravity_system.cpp
    ../nova_engine/src/physics_system/position_system.cpp
    ../nova_engine/src/event_system/controll_system.cpp
    ../nova_engine/src/event_system/ui_system.cpp
    ../nova_engine/src/core_utils.cpp
    ../nova_engine/src/graphical_system/draw_system.cpp
    ../nova_engine/src/event_system/keyboard_system.cpp
)

set(ZUMA
    src/main.cpp
    src/engine_init.cpp
)

add_executable(zuma ${ZUMA} ${ENGINE})

# Ajout de ZLIB::ZLIB pour r-type_client
target_link_libraries(zuma sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
