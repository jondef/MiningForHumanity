cmake_minimum_required(VERSION 3.15)
project(DonationMining)

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
# DonationMining
################################
add_subdirectory(DonationMining)