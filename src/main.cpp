#include "spdlog/spdlog.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>


// Constants zone
constexpr size_t MAX_RETURN_SIZE = 1024; // Using in exec() as text buffer size

const std::string SUCCESS_REMOVED = "Success";
const std::string SUCCESS_LACK = "Failure [not installed for 0]";

// Function to execute a command and get its output
std::string exec(const std::string &cmd) {
    std::array<char, MAX_RETURN_SIZE> buffer{}; // Buffer for reading data
    std::string result;

    // Open process using popen and ensure proper closure with unique_ptr
    const std::unique_ptr<FILE, int (*)(FILE *)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen failed!"); // Throw an error if popen fails
    }

    // Read data from the pipe and accumulate it into the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}


void removeRequest(const std::list<std::string> &REQUESTED_PACKAGES) {
    for (const std::string &package : REQUESTED_PACKAGES) {
        std::string command = "adb shell pm uninstall --user 0 " + package;
        std::string response = exec(command);

        if (response.find(SUCCESS_REMOVED) != std::string::npos) {
            spdlog::info("Package {} has been removed successfully.", package);
        } else if (response.find(SUCCESS_LACK) != std::string::npos) {
            spdlog::warn("Package {} is not present.", package);
        } else {
            spdlog::critical("Unexpected response for package {}: {}", package, response);
        }
    }
}

int main() {
    const std::list<std::string> LIST_PACKAGES = {
        "com.samsung.android.smartswitchassistant",  // Samsung Smart Switch Assistant
        "com.samsung.android.themestore",            // Samsung Theme Store
        "com.samsung.android.themecenter",           // Samsung Theme Center
        "com.samsung.android.game.gos",              // Game Optimization Service (GOS) | ID play on phone(
        "com.samsung.android.game.gametools",        // Game Tools
        "com.samsung.android.game.gamehome",         // Game Launcher
        "com.samsung.android.kidsinstaller",         // Samsung Kids Installer
        "com.samsung.android.aircommandmanager",     // Air Command (S Pen menu) | S Pen doesnt support on A21S
        "com.samsung.android.app.appsedge",          // Apps Edge
        "com.samsung.android.app.updatecenter",      // Update Center | Support was ending
        "com.samsung.android.shortcutbackupservice", // Shortcut Backup Service
        "com.samsung.android.scloud",                // Samsung Cloud
        "com.samsung.android.app.sharelive",         // Samsung Live Sharing
        "com.samsung.android.dialer",                // Phone/Dialer     | From Google better
        "com.samsung.android.messaging",             // Samsung Messages | >
        "com.samsung.android.app.contacts",          // Samsung Contacts | >
        "com.sec.android.app.samsungapps",           // Galaxy Store
        "com.sec.android.easyMover.Agent",           // Smart Switch Agent
        "com.sec.android.app.chromecustomizations",  // Chrome Customizations
        "com.android.chrome",                        // Google Chrome
        "tv.sweet.player",                           // Sweet TV Player
        "com.netflix.partner.activation",            // Netflix Activation
        "com.microsoft.skydrive",                    // Microsoft OneDrive
        "com.facebook.services",                     // Facebook Services
        "com.facebook.katana",                       // Facebook App
        "com.facebook.appmanager",                   // Facebook App Manager
        "com.facebook.system",                       // Facebook System Services
        "com.einnovation.temu",                      // Temu Shopping App
        "com.google.android.apps.tachyon",           // Google Duo/Meet
        "com.netflix.mediaclient",                   // Netflix App
        "com.scopely.monopolygo",                    // Monopoly GO! Game
        "com.samsung.android.bbc.bbcagent",          // Samsung BBC Agent
        "com.samsung.android.privateshare",          // Samsung Private Share
        "com.aura.oobe.samsung.gl"                   // AppCloud
    };

    removeRequest(LIST_PACKAGES);

    return 0;
}


// TODO: Розібратись з ctest
// TODO: Зробити меню керування
