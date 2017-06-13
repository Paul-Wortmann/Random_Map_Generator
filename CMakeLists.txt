cmake_minimum_required(VERSION 2.8.9)
project(Random_Map_Generator)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

include_directories(common)
 
set(MAIN_SOURCE main.cpp)
file(GLOB SOURCES "common/*.cpp")
 
add_executable(Random_Map_Generator ${SOURCES} ${MAIN_SOURCE})
