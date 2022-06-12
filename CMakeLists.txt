cmake_minimum_required(VERSION 3.10)

project(RAYTRACING)

set(CMAKE_C_STANDARD  11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

find_package(SDL2 CONFIG REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} main.c)

# SDL2::SDL2main needs to come first to compile with GCC 10.3.0
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2)