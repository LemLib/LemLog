#pragma once

#include "lemlog/logger/logger.hpp"
#include <vector>

namespace logger {
/**
 * @brief SD card class. Outputs all data to the SD card connected to the brain.
 */
class SDCard : public Sink {
    public:
        SDCard(std::string filename = ".log", bool logTimestamp = true,
               int cacheSize = 10);
        void send(Level level, std::string topic, std::string message) override;
    private:
        std::string formatTimestamp(long long ms);

        // cache of messages
        std::vector<std::string> cache;

        std::string filename;
        bool logTimestamp;
        int cacheSize;
};
} // namespace logger