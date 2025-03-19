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

# find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
# find_package(Boost REQUIRED COMPONENTS system thread serialization)
find_package(ZLIB REQUIRED)  # Ajouté pour trouver zlib

add_library(binary_protocol src/server/binary_protocol.cpp)
target_include_directories(binary_protocol PUBLIC include)

add_library(compress_data src/server/compress_data.cpp)
target_include_directories(compress_data PUBLIC include)
# target_include_directories(compress PUBLIC compress) homer aide moi avec ceci stp
# target_link_libraries(r-type_client compress)

set(ENGINE
    ../nova_engine/src/physics_system/collide_detection_system.cpp
    ../nova_engine/src/physics_system/collision_apply_system.cpp
    ../nova_engine/src/physics_system/gravity_system.cpp
    ../nova_engine/src/physics_system/position_system.cpp
    ../nova_engine/src/event_system/controll_system.cpp
    ../nova_engine/src/role_system/behavioral_system.cpp
)

set(SERVER
    src/server/main.cpp
    src/server/serveur.cpp
    src/server/dead_actors.cpp
    src/server/create_player.cpp
    src/server/move.cpp
    src/server/shoot.cpp
    src/server/start_game_all_pos.cpp
    src/server/enemy.cpp
    src/server/spawn_enemy.cpp
    src/server/error_managment.cpp
    src/server/deconnexion.cpp
    src/server/next_level.cpp
    src/server/demo_network/engine_init_network.cpp
    src/server/demo_network/delete_weapon_system.cpp
    src/server/demo_network/demo_network.cpp
    src/server/demo_network/delete_enemy.cpp    
)

set(CLIENT
    src/client/client.cpp
    src/client/graphical_client/game_engine.cpp
    ../nova_engine/src/graphical_system/draw_system.cpp
    ../nova_engine/src/core_utils.cpp
    ../nova_engine/src/event_system/input_system.cpp
    ../nova_engine/src/event_system/keyboard_system.cpp
    ../nova_engine/src/role_system/snack_bar_system.cpp
    ../nova_engine/src/event_system/ui_system.cpp
    src/client/graphical_client/game_system/delete_weapon_system.cpp
    src/client/graphical_client/game_system/delete_enemy.cpp
    src/client/graphical_client/utility/utility.cpp
    src/client/graphical_client/utility/utility_survival.cpp
    src/client/graphical_client/engine_init.cpp
)

add_executable(r-type_server ${SERVER} ${ENGINE})
add_executable(r-type_client ${CLIENT} ${ENGINE})

# Ajout de ZLIB::ZLIB pour r-type_client
target_link_libraries(r-type_server binary_protocol compress_data -lboost_system -lboost_thread -lboost_serialization -llua ZLIB::ZLIB)
target_link_libraries(r-type_client binary_protocol compress_data -lboost_system -lboost_thread -lboost_serialization sfml-system sfml-window sfml-graphics sfml-network sfml-audio -llua ZLIB::ZLIB)
