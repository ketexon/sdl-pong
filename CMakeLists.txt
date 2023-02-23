cmake_minimum_required(VERSION 3.25)

project(Pong)

add_executable(
    Pong
    src/Main.cpp

    src/App.hpp src/App.cpp
)

set_target_properties(
    Pong
    PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED True
)

set(SDL_LIB_DIR "" CACHE PATH "Library directory containing SDL2.lib")
set(SDL_DLL_PATH "" CACHE PATH "Path of SDL2.DLL")
set(SDL_INCLUDE_DIR "" CACHE PATH "Include directory containing SDL")

target_link_directories(
    Pong
    PRIVATE ${SDL_LIB_DIR}
)

target_include_directories(
    Pong
    PRIVATE ${SDL_INCLUDE_DIR}
)

target_link_libraries(
    Pong
    PRIVATE SDL2 SDL2main
)

add_custom_command(
    TARGET Pong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL_DLL_PATH}"
        "${CMAKE_CURRENT_BINARY_DIR}"
)