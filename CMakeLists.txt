cmake_minimum_required(VERSION 3.16)
project(ProjectTP)

set(CMAKE_CXX_STANDARD 14)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(game main.cpp)