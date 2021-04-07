cmake_minimum_required(VERSION 3.11.0)
project(Debuger)
file(GLOB_RECURSE DEBUGER_SRC main.cpp)
add_executable(debuger ${DEBUGER_SRC})