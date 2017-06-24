cmake_minimum_required(VERSION 2.8.7)
project(Random_Map_Generator)

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra -Wunused -Wshadow -Woverloaded-virtual -Wunreachable-code -Wconversion ${CMAKE_CXX_FLAGS}")

include_directories(src)
 
set(MAIN_SOURCE main.cpp)
file(GLOB SOURCES "src/*.cpp")
 
add_executable(Random_Map_Generator ${SOURCES} ${MAIN_SOURCE})

