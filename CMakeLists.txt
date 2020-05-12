cmake_minimum_required(VERSION 3.15)
project(DonationMining)

set(CMAKE_VERBOSE_MAKEFILE OFF)
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --coverage")
endif ()

function(print_all_variables)
	get_cmake_property(_variableNames VARIABLES)
	list(SORT _variableNames)
	foreach (_variableName ${_variableNames})
		message(STATUS "${_variableName}=${${_variableName}}")
	endforeach ()
endfunction()

set(EXECUTABLE_OUTPUT_PATH "bin" CACHE STRING "Path to place executables relative to ${CMAKE_INSTALL_PREFIX}")
set(LIBRARY_OUTPUT_PATH "bin" CACHE STRING "Path to place libraries relative to ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

################################
# xmrig
################################
add_subdirectory(xmrig)

################################
# BCrypt
################################
add_subdirectory(libbcrypt)

################################
# DonationMining
################################
add_subdirectory(DonationMining)

