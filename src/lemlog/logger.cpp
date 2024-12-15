#include "lemlog/logger.hpp"
#include <list>

namespace logger {

static std::list<Sink*> sinks;

Sink::Sink() { sinks.push_back(this); }

Sink::~Sink() { sinks.remove(this); }
} // namespace logger
