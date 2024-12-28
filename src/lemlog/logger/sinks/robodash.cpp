#include "lemlog/logger/sinks/robodash.hpp"

namespace logger {
Robodash::Robodash(rd_view_t* view)
    : view(view) {}

void Robodash::send(Level level, std::string topic, std::string message) {
    // for now, we only send errors to robodash
    if (level != Level::ERROR) return;

    std::string output = "[" + topic + "] " + message;

    rd_view_alert(this->view, output.c_str());
}
} // namespace logger