cmake_minimum_required(VERSION 3.10)

project(RAYTRACING)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

find_package(SDL2 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_FILES
    "src/*.h"
    "src/*.c"
)

# foreach(name ${SOURCE_FILES})
# message(STATUS "${name}")
# endforeach()
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# SDL2::SDL2main needs to come first to compile with GCC 10.3.0
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2)

if(MSVC)
    # https://docs.microsoft.com/en-us/previous-versions/thxezb7y(v=vs.140)?redirectedfrom=MSDN
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /wd4100) # unreferenced local parameter
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
