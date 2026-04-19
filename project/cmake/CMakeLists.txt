cmake_minimum_required(VERSION 3.0.0)
project(piece-hal VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

include_directories(../../src)
include_directories(../../include)

file(GLOB_RECURSE sources ../../src/*.h ../../src/*.cpp)
file(GLOB_RECURSE sources ../../src/*/*.h ../../src/*/*.cpp)

add_library(piece-hal ${sources})
target_link_libraries(piece-hal)
