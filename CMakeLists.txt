cmake_minimum_required(VERSION 2.8.7)
project(Random_Map_Generator)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wunused -Wshadow -Woverloaded-virtual -Wunreachable-code -Wconversion ${CMAKE_CXX_FLAGS}")

include_directories(common)
 
set(MAIN_SOURCE main.cpp)
file(GLOB SOURCES "common/*.cpp")
 
add_executable(Random_Map_Generator ${SOURCES} ${MAIN_SOURCE})

