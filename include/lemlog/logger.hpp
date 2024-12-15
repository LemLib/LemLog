#pragma once

#include <string>

namespace logger {

enum class Level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

/**
 * @brief Sink base class
 *
 */
class Sink {
    public:
        /**
         * @brief Construct a new Sink object
         *
         * This constructor adds the sink to the list of sinks that will be
         * accessed whenever a message is sent
         */
        Sink();
        /**
         * @brief Send a message via the sink
         *
         * derived classes may choose to only send messages with a specific
         * topic or logging level
         *
         * @param level the logging level of the message
         * @param topic the topic of the message, e.g
         *                  "lemlib/motions/move-to-point"
         * @param message the message to be sent
         */
        virtual void send(Level level, std::string topic,
                          std::string message) = 0;
        /**
         * @brief Destroy the Sink object
         *
         * This destructor removes the sink from the list of sinks that will
         * be accessed whenever a message is sent
         */
        virtual ~Sink();
};
} // namespace logger