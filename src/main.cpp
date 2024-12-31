#include "spdlog/spdlog.h"
#include <array>
#include <list>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>


// Constants zone
constexpr size_t MAX_RETURN_SIZE = 1024; // Using in exec() as text buffer size

const std::string SUCCESS_REMOVED = "Success";
const std::string SUCCESS_LACK = "Failure [not installed for 0]";

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


void removeRequest(const std::list<std::string>& REQUESTED_PACKAGES) {
    for (const std::string& package : REQUESTED_PACKAGES) {
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
        "com.samsung.android.smartswitchassistant",
        "com.samsung.android.themestore",
        "com.samsung.android.themecenter",
        "com.samsung.android.game.gos",
        "com.samsung.android.game.gametools",
        "com.samsung.android.game.gamehome",
        "com.samsung.android.kidsinstaller",
        "com.samsung.android.aircommandmanager",
        "com.samsung.android.app.appsedge",
        "com.samsung.android.app.updatecenter",
        "com.samsung.android.shortcutbackupservice",
        "com.samsung.android.scloud",
        "com.samsung.android.app.sharelive",
        "com.samsung.android.dialer",
        "com.samsung.android.messaging",
        "com.samsung.android.app.contacts",
        "com.samsung.android.game.gamehome",
        "com.sec.android.app.samsungapps",
        "com.sec.android.easyMover.Agent",
        "com.sec.android.app.chromecustomizations",
        "com.android.chrome",
        "tv.sweet.player",
        "com.netflix.partner.activation",
        "com.microsoft.skydrive",
        "com.facebook.services",
        "com.facebook.katana",
        "com.facebook.appmanager",
        "com.facebook.system",
        "com.einnovation.temu",
        "com.google.android.apps.tachyon",
        "com.netflix.mediaclient",
        "com.scopely.monopolygo",
        "com.samsung.android.bbc.bbcagent",
        "com.samsung.android.privateshare"
    };

    removeRequest(LIST_PACKAGES);

    return 0;
}



// TODO: Розібратись з ctest
// TODO: Зробити меню керування
