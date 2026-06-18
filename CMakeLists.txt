cmake_minimum_required(VERSION 3.20)

project(chip-8-emu)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    chip-8-emu
    src/main.cpp
    src/Chip8.cpp
)

target_compile_options(
    chip-8-emu
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
