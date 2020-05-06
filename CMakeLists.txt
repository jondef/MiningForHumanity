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

################################
# xmr-stak
################################
add_subdirectory(xmr-stak)

################################
# BCrypt
################################
add_subdirectory(libbcrypt)

################################
# DonationMining
################################
add_subdirectory(DonationMining)

