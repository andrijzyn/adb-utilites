#include <array>
#include <cstdio>
#include <curses.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

// Constants zone
constexpr size_t MAX_RETURN_SIZE = 128;
const std::string SUCCESS_REMOVED = "Success";
const std::string SUCCESS_LACK = "Failure [not installed for 0]";
const std::string DEVICES_LACK = "adb: no devices/emulators found";

// Enum to represent menu options with a smaller base type (std::uint8_t)
enum class MenuOption : std::uint8_t { GREET = 0, CLEAN, EXIT };

// Package list
const std::vector<std::string> PACKAGES = {
    "com.samsung.android.smartswitchassistant",  // Samsung Smart Switch Assistant
    "com.samsung.android.themestore",            // Samsung Theme Store
    "com.samsung.android.game.gametools",        // Game Tools
    "com.samsung.android.game.gamehome",         // Game Launcher
    "com.samsung.android.kidsinstaller",         // Samsung Kids Installer
    "com.samsung.android.app.appsedge",          // Apps Edge
    "com.samsung.android.app.updatecenter",      // Update Center
    "com.samsung.android.shortcutbackupservice", // Shortcut Backup Service
    "com.samsung.android.scloud",                // Samsung Cloud
    "com.samsung.android.app.sharelive",         // Samsung Live Sharing
    "com.samsung.android.dialer",                // Phone/Dialer
    "com.samsung.android.messaging",             // Samsung Messages
    "com.samsung.android.app.contacts",          // Samsung Contacts
    "com.samsung.sree",                          // Samsung Services
    "com.samsung.android.app.tips",              // Samsung Tips
    "com.samsung.android.app.parentalcare",      // Parental Control
    "com.samsung.android.aremoji",               // Samsung AR Emoji
    "com.sec.android.app.samsungapps",           // Galaxy Store
    "com.sec.android.easyMover.Agent",           // Smart Switch Agent
    "com.sec.android.app.chromecustomizations",  // Chrome Customizations
    "com.android.chrome",                        // Google Chrome
    "tv.sweet.player",                           // Sweet TV Player
    "com.megogo.application",                    // MEGOGO
    "com.netflix.partner.activation",            // Netflix Activation
    "com.microsoft.skydrive",                    // OneDrive
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

// Function to execute a command and get its output
std::string exec(const std::string &cmd) {
    std::array<char, MAX_RETURN_SIZE> buffer{}; // Buffer for reading data
    std::string result;

    // Open process using popen and ensure proper closure with unique_ptr
    const std::unique_ptr<FILE, int (*)(FILE *)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        spdlog::error("Command execution failed: {}", cmd);    // Log error if popen fails
        throw std::runtime_error("Command execution failed!"); // Throw an error if popen fails
    }

    // Read data from the pipe and accumulate it into the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

// Function to check if adb devices are available
bool checkDevicesAvailable() {
    std::string response = exec("adb shell");

    // Check if the response contains any device entry
    if (response.find("adb: no devices/emulators found") == std::string::npos) {
        spdlog::critical("No devices or emulators found. Please connect a device.");
        return false; // No devices found
    }

    return true; // Devices are available
}

void removeRequest() {
    // First, check if devices are available
    if (!checkDevicesAvailable()) {
        return; // Exit the function if no devices are found
    }

    // Proceed with package removal if devices are found
    for (const std::string &package : PACKAGES) {
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

// Function to display the menu and get user choice
MenuOption displayMenu(const std::vector<std::string> &options) {
    int highlight = 0;
    int choice = -1; // To store the user choice

    while (choice == -1) {
        clear(); // Clear screen

        // Display menu options
        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            } // Highlight selected option
            mvprintw(i + 1, 2, options[i].c_str());
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }

        // Handle key press
        const int key = getch();
        switch (key) {
        case KEY_UP:
            highlight = (highlight == 0) ? options.size() - 1 : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % options.size();
            break;
        case '\n': // Enter key
            choice = highlight;
            break;
        default:
            break;
        }
    }

    return static_cast<MenuOption>(choice);
}

// Function to display a greeting
void showGreeting() {
    mvprintw(1, 2, "Hello, World!");
}

void waitForUserInput() {
    mvprintw(3, 2, "Press any key to return to menu...");
    getch(); // Wait for user input to return to menu
}


int main() {
    // Initialize spdlog (console logging with default level)
    spdlog::set_level(spdlog::level::info); // Set minimum log level to info

    initscr();            // Initialize ncurses
    cbreak();             // Disable input buffering
    noecho();             // Disable echoing typed characters
    keypad(stdscr, true); // Enable arrow keys

    // Define menu options
    const std::vector<std::string> options = {"Action 1: Say Hello", "Action 2: Clean", "Exit"};

    while (true) {
        // Display the menu and get user selection
        MenuOption choice = displayMenu(options);

        clear(); // Clear screen before executing action
        switch (choice) {
        case MenuOption::GREET:
            showGreeting();
            spdlog::info("Displayed greeting.");
            waitForUserInput(); // Return to menu after action
            break;
        case MenuOption::CLEAN:
            endwin(); // Exit ncurses mode before removing packages
            removeRequest();
            spdlog::info("Press any key to return to menu...");
            getchar(); // Wait for user input to return to menu
            initscr(); // Reinitialize ncurses before returning to the menu
            break;
        case MenuOption::EXIT:
            endwin(); // End ncurses session
            spdlog::info("Exiting program.");
            return 0;
        default:
            break;
        }
    }

    endwin(); // End ncurses session
    return 0;
}
