cmake_minimum_required(VERSION 3.13)

project(mcts-cpp VERSION 1.0 LANGUAGES CXX)

# set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_compile_options(-Wall -Wextra -pedantic)

add_library(MCTS)

set_target_properties(MCTS PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(include)
add_subdirectory(examples)

# enable_testing()

# add_subdirectory(test)
