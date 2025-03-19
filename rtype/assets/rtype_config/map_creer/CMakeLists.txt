cmake_minimum_required(VERSION 3.16)

project(qt_map LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver les bibliothèques Qt6 nécessaires
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Déclaration des fichiers sources et headers
set(SOURCES
    main.cpp
    JsonEditor.cpp
)

set(HEADERS
    JsonEditor.h
)

# Création de l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Lier les bibliothèques Qt6 (avec mots-clés)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
