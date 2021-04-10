cmake_minimum_required(VERSION 3.11.0)
project(Debuger)
include_directories(
	loger)
file(GLOB_RECURSE DEBUGER_SRC
	stdafx.h
	stdafx.cpp
	main.cpp
	loger/tloger.h
	loger/tloger.cpp)
add_executable(debuger ${DEBUGER_SRC})

SET(PCH_HEADER_FILE "stdafx.h")
SET(PCH_SOURCE_FILE "../Debuger/stdafx.cpp")
SET(PCH_DIR ${CMAKE_CURRENT_BINARY_DIR}/PCH)

MACRO(Add_PCH)
	FILE(MAKE_DIRECTORY ${PCH_DIR})
	SET_TARGET_PROPERTIES(debuger PROPERTIES COMPILE_FLAGS 
		"/Yu${PCH_HEADER_FILE} /FI${PCH_HEADER_FILE} /Fp${PCH_DIR}/Debuger.pch")
	SET_SOURCE_FILES_PROPERTIES(${PCH_SOURCE_FILE} PROPERTIES COMPILE_FLAGS
		"/Yc${PCH_HEADER_FILE}")
	SET_DIRECTORY_PROPERTIES(PROPERTIES
		ADDITIONAL_MAKE_CLEAN_FILES ${PCH_DIR}/Debuger.pch)
ENDMACRO()
Add_PCH()

execute_process(COMMAND cmake -P ../Debuger/cmake/debuger.cmake)
