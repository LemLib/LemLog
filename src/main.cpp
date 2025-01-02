#include "main.h"

#include "lemlog/logger/Helper.hpp"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    logger::Helper test("test-topic");

    test.log(logger::Level::INFO, "The value is {}", 42);
    test.log(logger::Level::INFO, "hello!");
}