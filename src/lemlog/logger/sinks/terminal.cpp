#include "lemlog/logger/sinks/terminal.hpp"
#include "pros/apix.h"

// DEBUG - GREEN
// INFO - CYAN
// WARN - ORANGE
// ERROR - RED

namespace logger {
Terminal::Terminal(bool disableCOBS, bool disableStreamID) {
    if (disableCOBS) { pros::c::serctl(SERCTL_DISABLE_COBS, NULL); }
    if (disableStreamID) {
        // 0x74756f73 is the stream identifier ("sout")
        pros::c::serctl(SERCTL_NOBLKWRITE, (void*)0x74756f73);
    }
}

void Terminal::send(Level level, std::string topic, std::string message) {
    // output: <reset> <color> [LEVEL] <gray> (topic) <reset> message
    std::string output = "\u001b[0m"; // reset code
    // logging level
    switch (level) {
        case (Level::DEBUG): {
            output += "\u001b[32[DEBUG]";
            break;
        }
        case (Level::INFO): {
            output += "\u001b[36[INFO]";
            break;
        }
        case (Level::WARN): {
            output += "\u001b[33[WARN]";
            break;
        }
        case (Level::ERROR): {
            output += "\u001b[31[ERROR]";
            break;
        }
    }
    // topic
    output += " \u001b[38;5;8m (" + topic + ")\u001b[0m";
    // message
    output += message += "\n";
    // print
    std::cout << output << std::flush;
}
} // namespace logger