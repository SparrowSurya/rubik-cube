cmake_minimum_required(VERSION 3.16)

project(app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable raylib examples and extras
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# Add raylib
add_subdirectory(external/raylib)

file(GLOB_RECURSE SOURCES src/*.c)

# Your executable
add_executable(app ${SOURCES})

target_include_directories(app PRIVATE
    src
    include
)
# Link raylib
target_link_libraries(app PRIVATE raylib)

# Linux needs these explicitly sometimes
if (UNIX AND NOT APPLE)
    target_link_libraries(app PRIVATE m dl pthread)
endif()
