cmake_minimum_required(VERSION 3.20)

project(chip-8-emu)

# use c++ 23
set(CMAKE_CXX_STANDARD 23)
# prevent falling back to older c++ versions
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# disable compiler specific c++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# create chip-8-emu target
add_executable(
    chip-8-emu
    src/main.cpp
    src/Chip8.cpp
    src/Platform.cpp
)

# enable extra compiler warnings
target_compile_options(
    chip-8-emu
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# locate system-installed SDL3
find_package(SDL3 REQUIRED)

# link sdl3 with chip-8-emu target
target_link_libraries(chip-8-emu
    PRIVATE
    SDL3::SDL3
)
