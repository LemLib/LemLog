#include "main.h"

#include "lemlog/logger/Helper.hpp"
#include "lemlog/logger/sinks/terminal.hpp"

logger::Terminal terminal;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    logger::Helper test("test-topic");

    // test formatting
    terminal.setLoggingLevel(logger::Level::DEBUG);
    test.log(logger::Level::INFO, "The value is {}", 42);
    test.log(logger::Level::DEBUG, "debug test");
    test.log(logger::Level::INFO, "info test");
    test.log(logger::Level::WARN, "warn test");
    test.log(logger::Level::ERROR, "error test");
    terminal.setLoggingLevel(logger::Level::INFO);

    // messages sent without a high enough level will not be sent
    test.log(logger::Level::DEBUG, "this message will not be sent");
    test.log(logger::Level::INFO, "this message will be sent");

    // messages with topics on the allowlist will have their messages sent no
    // matter the logging level
    terminal.addToAllowList("test-topic");
    terminal.setLoggingLevel(logger::Level::DEBUG);
    test.log(logger::Level::INFO, "this message is whitelisted, and should be "
                                  "sent despite being below the logging level");

    // messages with their topics on the blocklist will never have their
    // messages sent
    terminal.removeFromAllowList("test-topic");
    terminal.addToBlockedList("test-topic");
    test.log(logger::Level::INFO,
             "this message is on the blockedlist, and will not be sent despite "
             "having a high enough logging level");
    terminal.removeFromBlockedList("test-topic");
}