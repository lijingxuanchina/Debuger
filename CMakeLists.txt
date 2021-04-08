cmake_minimum_required(VERSION 3.11.0)
project(Debuger)
include_directories(
	loger)
file(GLOB_RECURSE DEBUGER_SRC 
	main.cpp
	loger/tloger.h
	loger/tloger.cpp)
add_executable(debuger ${DEBUGER_SRC})