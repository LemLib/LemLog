#pragma once

#include <string>

namespace logger {
class Sink {
    public:
        Sink();
        virtual void send(std::string topic, std::string message);
        virtual ~Sink();
};
} // namespace logger