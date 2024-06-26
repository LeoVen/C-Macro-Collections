cmake_minimum_required(VERSION 3.8)

project(cmc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE sources
    "./tests/main.c"
)

add_definitions(-DCMC_CALLBACKS)

add_executable(cmc ${sources})

target_include_directories(cmc PUBLIC
    "./"
)

