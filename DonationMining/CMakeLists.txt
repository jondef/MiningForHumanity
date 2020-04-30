set(CMAKE_CXX_STANDARD 20)
project(DonationMining)

################################
# Versioning
################################
# set version number
set(VERSION 0.1)
# get current date time
if (WIN32)
	execute_process(
			COMMAND date -R # date /t
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE BUILDDATE
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
elseif (UNIX)
	execute_process(
			COMMAND date -R
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE BUILDDATE
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
elseif (APPLE)
	execute_process(
			COMMAND date -R
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE BUILDDATE
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif ()
# Get git commit hash
execute_process(
		COMMAND git --git-dir ${CMAKE_SOURCE_DIR}/.git --work-tree ${CMAKE_SOURCE_DIR} rev-parse HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_COMMIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Get git commit date
execute_process(
		COMMAND git --git-dir ${CMAKE_SOURCE_DIR}/.git --work-tree ${CMAKE_SOURCE_DIR} show -s --format=%ci HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_COMMIT_DATE
		OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Get git branch
execute_process(
		COMMAND git --git-dir ${CMAKE_SOURCE_DIR}/.git --work-tree ${CMAKE_SOURCE_DIR} rev-parse --abbrev-ref HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_BRANCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DVERSION="${VERSION}")
if (NOT BUILDDATE STREQUAL "")
	add_definitions(-DBUILDDATE="${BUILDDATE}")
endif ()
if (NOT GIT_COMMIT_HASH STREQUAL "")
	add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
endif ()
if (NOT GIT_COMMIT_DATE STREQUAL "")
	add_definitions(-DGIT_COMMIT_DATE="${GIT_COMMIT_DATE}")
endif ()
if (NOT GIT_BRANCH STREQUAL "")
	add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")
endif ()

################################
# Qt configuration
################################

if (WIN32)
	message("WIN32")
	add_definitions(-DQT_STATICPLUGIN)
	set(USE_STATIC_QT_BY_DEFAULT ON)
	set(QT_PLUGINS_DIR /usr/x86_64-w64-mingw32/lib/qt/plugins)
	link_directories("${QT_PLUGINS_DIR}/platforms")
	link_directories("${QT_PLUGINS_DIR}/imageformats")
	link_directories("/usr/x86_64-w64-mingw32/lib/")
	#	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif ()

FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets Charts Network PrintSupport Test QuickTest REQUIRED)

foreach (plugin ${Qt5Gui_PLUGINS})
	get_target_property(_loc ${plugin} LOCATION)
	message("Plugin ${plugin} is at location ${_loc}")
	set(plugin_libs ${plugin_libs} ${_loc})
endforeach ()

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files
set(CMAKE_AUTORCC ON) # Create Resource files
SET(QT_USE_QTNETWORK true)

################################
# Pre-compilation steps
################################
include_directories(
		#	${PROJECT_SOURCE_DIR}/Headers
		${PROJECT_SOURCE_DIR}/src
)

file(GLOB SOURCE_FILES
		${PROJECT_SOURCE_DIR}/src/*.cpp
		${PROJECT_SOURCE_DIR}/src/*.c
		)

#file(GLOB HEADER_FILES
#		"${PROJECT_SOURCE_DIR}/Headers/*.h"
#		"${PROJECT_SOURCE_DIR}/Headers/*.hpp"
#		)

file(GLOB UI_FILES
		"${PROJECT_SOURCE_DIR}/src/*.ui"
		)

file(GLOB QRC_FILES
		"${PROJECT_SOURCE_DIR}/Resources/*.qrc"
		)

################################
# Compiling
################################
qt5_wrap_ui(ui_wrap ${UI_FILES})

if (WIN32)
	add_executable(DonationMining WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${ui_wrap} ${QRC_FILES})
else ()
	add_executable(DonationMining ${SOURCE_FILES} ${HEADER_FILES} ${ui_wrap} ${QRC_FILES})
endif ()

set(EXECUTABLE_OUTPUT_PATH "bin" CACHE STRING "Path to place executables relative to ${CMAKE_INSTALL_PREFIX}")
set(LIBRARY_OUTPUT_PATH "bin" CACHE STRING "Path to place libraries relative to ${CMAKE_INSTALL_PREFIX}")

################################
# Linking
################################
target_link_libraries(DonationMining Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Charts Qt5::Network)
if (WIN32)
	target_link_libraries(DonationMining Qt5::QWindowsIntegrationPlugin Qt5::QGenericEnginePlugin Qt5::QJpegPlugin Qt5::QGifPlugin Qt5::QICOPlugin)
endif ()
