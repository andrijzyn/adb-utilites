#include <spdlog/spdlog.h>
#include <array>
#include <cstdio>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>


// Constants zone
constexpr size_t MAX_RETURN_SIZE = 1024;
const std::string SUCCESS_REMOVED = "Success";
const std::string SUCCESS_LACK = "Failure [not installed for 0]";
const std::vector<std::string> VECTOR_PACKAGES = {
    "com.samsung.android.smartswitchassistant", // Samsung Smart Switch Assistant
    "com.samsung.android.themestore",           // Samsung Theme Store
    "com.samsung.android.game.gos",             // Game Optimization Service (GOS)
    "com.samsung.android.game.gametools",       // Game Tools
    "com.samsung.android.game.gamehome",        // Game Launcher
    "com.samsung.android.kidsinstaller",        // Samsung Kids Installer
    "com.samsung.android.app.appsedge",         // Apps Edge
    "com.samsung.android.app.updatecenter",     // Update Center
    "com.samsung.android.shortcutbackupservice",// Shortcut Backup Service
    "com.samsung.android.scloud",               // Samsung Cloud
    "com.samsung.android.app.sharelive",        // Samsung Live Sharing
    "com.samsung.android.dialer",               // Phone/Dialer
    "com.samsung.android.messaging",            // Samsung Messages
    "com.samsung.android.app.contacts",         // Samsung Contacts
    "com.samsung.sree",                         // Samsung Services
    "com.samsung.android.app.tips",             // Samsung Tips
    "com.samsung.android.app.parentalcare",     // Parental Control
    "com.google.android.tts",                   // Google TTS
    "com.samsung.android.aremoji",              // Samsung AR Emoji
    "com.sec.android.app.samsungapps",          // Galaxy Store
    "com.sec.android.easyMover.Agent",          // Smart Switch Agent
    "com.sec.android.app.chromecustomizations", // Chrome Customizations
    "com.android.chrome",                      // Google Chrome
    "tv.sweet.player",                         // Sweet TV Player
    "com.megogo.application",                  // MEGOGO
    "com.netflix.partner.activation",          // Netflix Activation
    "com.microsoft.skydrive",                  // OneDrive
    "com.facebook.services",                   // Facebook Services
    "com.facebook.katana",                     // Facebook App
    "com.facebook.appmanager",                 // Facebook App Manager
    "com.facebook.system",                     // Facebook System Services
    "com.einnovation.temu",                    // Temu Shopping App
    "com.google.android.apps.tachyon",         // Google Duo/Meet
    "com.netflix.mediaclient",                 // Netflix App
    "com.scopely.monopolygo",                  // Monopoly GO! Game
    "com.samsung.android.bbc.bbcagent",        // Samsung BBC Agent
    "com.samsung.android.privateshare",        // Samsung Private Share
    "com.aura.oobe.samsung.gl"                 // AppCloud
};

// Function to execute a command and get its output
std::string exec(const std::string &cmd) {
    std::array<char, MAX_RETURN_SIZE> buffer{}; // Buffer for reading data
    std::string result;

    // Open process using popen and ensure proper closure with unique_ptr
    const std::unique_ptr<FILE, int (*)(FILE *)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Command execution failed!"); // Throw an error if popen fails
    }

    // Read data from the pipe and accumulate it into the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

// Function to remove packages
void removeRequest() {
    for (const std::string &package : VECTOR_PACKAGES) {
        std::string response = exec("adb shell pm uninstall --user 0 " + package);

        // If package successfully has been removed
        if (response.find(SUCCESS_REMOVED) != std::string::npos) {
            spdlog::info("Package {} has been removed successfully.", package);
        }
        // If package hasn't been removed
        else if (response.find(SUCCESS_LACK) != std::string::npos) {
            spdlog::warn("Package {} is not present.", package);
        }
        // Unexpected response
        else {
            spdlog::critical("Unexpected response for package {}: {}", package, response);
        }
    }
}


int main() {
    removeRequest();

    return 0;
}


    // Uncomment for testing package removal function
    // removeRequest();
}
