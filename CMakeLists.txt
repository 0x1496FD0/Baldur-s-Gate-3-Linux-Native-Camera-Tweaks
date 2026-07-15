cmake_minimum_required(VERSION 4.3.4)
project(linux_native_camera_tweaks)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.c" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src ${SDL2_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE _GNU_SOURCE)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
