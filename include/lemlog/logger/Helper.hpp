#pragma once

#include <format>
#include <string>
#include "lemlog/logger/Sink.hpp"

namespace logger {
/**
 * @brief Logger Helper class. Used to send messages to all sinks
 *
 */
class Helper {
        friend void log(Level level, const std::string& topic,
                        const std::string& message);
    public:
        /**
         * @brief Construct a new Helper object
         *
         * @param topic the topic messages will be sent under
         *
         * @b Example:
         * @code {.cpp}
         *   // create a Helper, under the topic doSomething
         *   logger::Helper helper("doSomething");
         *   // log an info message, under te topic doSomething
         *   helper.log(logger::Level::INFO, "Did something!");
         * }
         * @endcode
         */
        Helper(const std::string& topic);

        /**
         * @brief Send a message to all sinks
         *
         * Sends a message to all sinks
         *
         * @param level the logging level of the message
         * @param format the format of the message to be sent
         * @param args the arguments to be formatted into the message
         *
         * @b Example:
         * @code {.cpp}
         * void doSomething() {
         *   // create a Helper
         *   logger::Helper helper("doSomething");
         *   // log an info message
         *   helper.log(logger::Level::INFO, "Motor temperature: {}", 42);
         * }
         * @endcode
         */
        template <typename... Args>
        void log(Level level, std::string_view format, Args&&... args) const {
            const auto& formatted_args = std::make_format_args(args...);

            // Create the format_args object using std::forward
            const std::string& message = std::vformat(format, formatted_args);

            // Log the formatted message
            write(level, m_topic, message);
        }
    private:
        /**
         * @brief
         *
         * @param level
         * @param topic
         * @param message
         */
        void write(Level level, const std::string& topic,
                   const std::string& message) const;
        const std::string m_topic;
};
} // namespace logger