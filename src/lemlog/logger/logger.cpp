#include "lemlog/logger/logger.hpp"
#include <list>

namespace logger {

// doubly linked list of all sinks
static std::list<Sink*> sinks;
static std::list<std::string> whitelist;
static std::list<std::string> blacklist;

void addWhitelist(std::string s) { whitelist.push_back(s); }

void removeWhitelist(std::string s) { whitelist.remove(s); }

void addBlacklist(std::string s) { blacklist.push_back(s); }

void removeBlacklist(std::string s) { blacklist.remove(s); }

/**
 * @brief log a message with a level and topic
 *
 * Messages with a logging level of DEBUG must have the topic whitelisted,
 * while all other messages are sent by default unless the topic is blacklisted
 *
 * @param level the level of the message, e.g INFO
 * @param topic the topic of the message, e.g "lemlib/motions/boomerang"
 * @param message the message to send, e.g "Hello World!"
 */
static void log(Level level, std::string topic, std::string message) {
    // is the message a debug message?
    if (level == Level::DEBUG) {
        // is it whitelisted?
        bool found = false;
        for (const std::string& s : whitelist) {
            if (topic == s) {
                found = true;
                break;
            }
        }
        if (!found) return;
    } else {
        // is the message blacklisted?
        for (const std::string& s : blacklist) {
            if (topic == s) return;
        }
    }
    // send the message to all sinks
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
