#pragma once

#include <string>

namespace logger {
class Sink {
public:
  virtual void send(std::string topic, std::string message);
};
} // namespace logger