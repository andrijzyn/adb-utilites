#include <spdlog/spdlog.h>
#include <array>
#include <cstdio>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <curses.h>

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

// Function to display menu and select action
int displayMenu(const std::vector<std::string>& options) {
    int highlight = 0; // Highlighted option

    while (true) {
        clear(); // Clear screen
        for (size_t i = 0; i < options.size(); ++i) {
            if (i == highlight)
                attron(A_REVERSE); // Highlight current option
            mvprintw(i + 1, 2, options[i].c_str());
            if (i == highlight)
                attroff(A_REVERSE);
        }

        int key = getch(); // Get key input
        switch (key) {
            case KEY_UP:
                highlight = (highlight > 0) ? highlight - 1 : options.size() - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % options.size();
                break;
            case '\n': // Enter key
                return highlight;
        }
    }
}

int main() {
    initscr();            // Initialize ncurses
    cbreak();             // Disable input buffering
    noecho();             // Disable echoing typed characters
    keypad(stdscr, true); // Enable arrow keys

    std::vector<std::string> options = {
        "Action 1: Say Hello",
        "Action 2: Show Time",
        "Exit"
    };

    while (true) {
        int choice = displayMenu(options);

        clear(); // Clear screen before executing action
        if (choice == 0) {
            mvprintw(1, 2, "Hello, World!");
        } else if (choice == 1) {
            mvprintw(1, 2, "Current time is: ");
            system("date"); // Execute system command
        } else if (choice == 2) {
            break; // Exit program
        }

        mvprintw(3, 2, "Press any key to return to menu...");
        getch(); // Wait for user input
    }

    endwin(); // End ncurses session
    return 0;

    // Uncomment for testing package removal function
    // removeRequest();
}
