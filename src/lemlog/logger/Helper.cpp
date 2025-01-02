#include "lemlog/logger/Helper.hpp"

namespace logger {
Helper::Helper(const std::string& topic)
    : m_topic(topic) {}

void log(Level level, const std::string& topic, const std::string& message);

void Helper::write(Level level, const std::string& topic,
                   const std::string& message) const {
    log(level, topic, message);
}
} // namespace logger