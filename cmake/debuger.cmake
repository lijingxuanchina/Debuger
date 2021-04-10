SET(CMAKE_SCRIPTS_PATH "${CMAKE_CURRENT_BINARY_DIR}/../Debuger/cmake")

INCLUDE(${CMAKE_SCRIPTS_PATH}/addpch.cmake)

MACRO(RUN_CMAKE_SCRIPTS)
	message("-- Run cmake scripts")

	# This macro from cmake/addpch.cmake
	ADD_PCH()
ENDMACRO(RUN_CMAKE_SCRIPTS)