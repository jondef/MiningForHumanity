set(CMAKE_CXX_STANDARD 20)
project(DonationMining)

################################
# Qt configuration
################################
FIND_PACKAGE(Qt5 COMPONENTS Core Gui Widgets Network PrintSupport Test QuickTest REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files
set(CMAKE_AUTORCC ON) # Create Resource files

################################
# Pre-compilation steps
################################
include_directories(
        #		${PROJECT_SOURCE_DIR}/Headers
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
qt5_wrap_ui(ui_wrap MainWindow.ui)

add_executable(DonationMining ${SOURCE_FILES} ${HEADER_FILES} ${ui_wrap} ${QRC_FILES})


set(EXECUTABLE_OUTPUT_PATH "bin" CACHE STRING "Path to place executables relative to ${CMAKE_INSTALL_PREFIX}")
set(LIBRARY_OUTPUT_PATH "bin" CACHE STRING "Path to place libraries relative to ${CMAKE_INSTALL_PREFIX}")

################################
# Linking
################################
target_link_libraries(DonationMining PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
