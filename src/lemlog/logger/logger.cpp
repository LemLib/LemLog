#include "lemlog/logger/logger.hpp"
#include <list>

namespace logger {

static std::list<Sink*> sinks;

Helper::Helper(std::string topic)
    : m_topic(topic) {}

void Helper::log(Level level, std::string message) {
    // send a message to every sink
    for (Sink* const sink : sinks) {
        sink->send(level, m_topic, message);
    }
}

Sink::Sink() { sinks.push_back(this); }

Sink::~Sink() { sinks.remove(this); }
} // namespace logger
