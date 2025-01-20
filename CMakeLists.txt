cmake_minimum_required(VERSION 3.10.0)
project(utilites LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libraries
find_package(spdlog REQUIRED)
find_package(Curses REQUIRED)

# Add executable
add_executable(utilites src/main.cpp)
target_compile_options(utilites PRIVATE -Wall -Wextra -O2)

# Linking with spdlog and Curses
target_link_libraries(utilites PRIVATE spdlog::spdlog ${CURSES_LIBRARIES})
