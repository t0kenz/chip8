cmake_minimum_required(VERSION 3.26)

project(chip8)
add_subdirectory(src)

find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
find_library(SDL2_LIBRARY NAME SDL2)

