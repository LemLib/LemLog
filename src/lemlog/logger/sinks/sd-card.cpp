#include "lemlog/logger/sinks/sd-card.hpp"
#include "pros/apix.h"
#include <fstream>

namespace logger {
SDCard::SDCard(std::string filename, bool logTimestamp) : filename("/usd/" + filename), logTimestamp(logTimestamp) {}

std::string SDCard::formatTimestamp(long long ms) {
    // use the % operator to get the remainder of the division
    long long minutes = ms / 60000;
    ms %= 60000;
    long long seconds = ms / 1000;
    ms %= 1000;

    std::string output = "";

    if (minutes > 0) output += std::to_string(minutes) + "m ";
    if (seconds > 0 || minutes > 0) output += std::to_string(seconds) + "s ";
    output += std::to_string(ms) + "ms";

    return output;
}

void SDCard::send(Level level, std::string topic, std::string message) {
    // output: <time> [LEVEL] (topic) message
    std::string output = "";

    if (this->logTimestamp) output += this->formatTimestamp((long long) pros::millis());

    switch (level) {
        case (Level::DEBUG): {
            output += " [DEBUG]";
            break;
        }
        case (Level::INFO): {
            output += " [INFO]";
            break;
        }
        case (Level::WARN): {
            output += " [WARN]";
            break;
        }
        case (Level::ERROR): {
            output += " [ERROR]";
            break;
        }
    }

    output += " (" + topic + ") " + message + "\n";

    std::ofstream file(this->filename);
    file << output;
    file.close();
}
}