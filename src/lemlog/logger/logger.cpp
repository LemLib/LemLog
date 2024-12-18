#include "lemlog/logger/logger.hpp"
#include <list>

namespace logger {

static std::list<Sink*> sinks;

static void log(Level level, std::string topic, std::string message) {
    // send a message to every sink
    for (Sink* const sink : sinks) {
        sink->send(level, topic, message);
    }
}

Helper::Helper(std::string topic)
    : m_topic(topic) {}

void Helper::log(Level level, std::string message) {
    logger::log(level, m_topic, message);
}

Sink::Sink() { sinks.push_back(this); }

Sink::~Sink() { sinks.remove(this); }
} // namespace logger
