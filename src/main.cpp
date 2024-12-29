#include "spdlog/spdlog.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>


// Constants zone
constexpr size_t MAX_RETURN_SIZE = 1024; // Using in exec() as text buffer size


// Function to execute a command and get its output
std::string exec(const std::string& cmd) {
    std::array<char, MAX_RETURN_SIZE> buffer{};  // Buffer for reading data
    std::string result;

    // Open process using popen and ensure proper closure with unique_ptr
    const std::unique_ptr<FILE, int(*)(FILE*)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen failed!");  // Throw an error if popen fails
    }

    // Read data from the pipe and accumulate it into the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}


void spdlog_test() {
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    // Compile time log levels
    // Note that this does not change the current log level, it will only
    // remove (depending on SPDLOG_ACTIVE_LEVEL) the call on the release code.
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");
}

int main() {
    // spdlog_test();

    std::cout << exec("tree") << std::endl;
}



// TODO: Розібратись з ctest
