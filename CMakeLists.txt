INCLUDE(cmake/debuger.cmake)

cmake_minimum_required(VERSION 3.11.0)
project(Debuger)
include_directories(
	loger)
file(GLOB_RECURSE DEBUGER_SRC
	stdafx.h
	stdafx.cpp
	main.cpp
	resultinfo.h
	tdebuger.h
	tdebuger.cpp
	toutputhelper.h
	toutputhelper.cpp
	tlifespanhelper.h
	tlifespanhelper.cpp
	loger/tloger.h
	loger/tloger.cpp)
add_definitions(-DUNICODE -D_UNICODE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows") 
add_executable(debuger ${DEBUGER_SRC})

RUN_CMAKE_SCRIPTS()
