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
# Compiling
################################
qt5_wrap_ui(ui_wrap MainWindow.ui)
add_executable(DonationMining src/main.cpp src/MainWindow.cpp src/MainWindow.h ${ui_wrap})

set(EXECUTABLE_OUTPUT_PATH "bin" CACHE STRING "Path to place executables relative to ${CMAKE_INSTALL_PREFIX}")
set(LIBRARY_OUTPUT_PATH "bin" CACHE STRING "Path to place libraries relative to ${CMAKE_INSTALL_PREFIX}")

################################
# Linking
################################
target_link_libraries(DonationMining PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
