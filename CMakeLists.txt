cmake_minimum_required(VERSION 3.10.0)
project(smsng_cleaner VERSION 0.1.0 LANGUAGES C CXX)

# Set stndrt C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Docs needed
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/Doxyfile
  COMMAND doxygen -g
  COMMENT "Generating Doxyfile"
)

find_package(spdlog REQUIRED)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# mail exec file
add_executable(cleaner src/main.cpp)

# Linking with spdlog
target_link_libraries(cleaner PRIVATE spdlog::spdlog ${CURSES_LIBRARIES})
