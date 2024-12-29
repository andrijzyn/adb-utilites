cmake_minimum_required(VERSION 3.10.0)
project(smsng_cleaner VERSION 0.1.0 LANGUAGES C CXX)

# Set stndrt C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Docs needed
add_custom_target(docs
  COMMAND doxygen
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Finding lib's: spdlog
find_package(spdlog REQUIRED)

# Options for coverage code
option(ENABLE_COVERAGE "Enable code coverage reporting" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling coverage flags")
        add_compile_options(--coverage)
        add_link_options(--coverage)
    else()
        message(WARNING "Code coverage is not supported on this compiler")
    endif()
endif()

# mail exec file
add_executable(cleaner main.cpp)

# Linking with spdlog
target_link_libraries(cleaner PRIVATE spdlog::spdlog)

# Target for generating report for Covering
if(ENABLE_COVERAGE)
    find_program(GCOVR_EXECUTABLE gcovr)
    if(GCOVR_EXECUTABLE)
        add_custom_target(coverage
            COMMAND ${GCOVR_EXECUTABLE} -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage.html
            COMMAND ${GCOVR_EXECUTABLE} -r ${CMAKE_SOURCE_DIR} --xml -o coverage.xml
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating coverage report"
        )
    else()
        message(WARNING "gcovr not found! Coverage report will not be generated.")
    endif()
endif()
