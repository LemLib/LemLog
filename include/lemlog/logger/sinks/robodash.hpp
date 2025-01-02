#pragma once

#include "lemlog/logger/logger.hpp"
#include "robodash/core.h"

namespace logger {
/**
 * @brief Robodash class. Outputs errors to the Robodash GUI Alert API
 */
class Robodash : public Sink {
    public:
        Robodash(rd_view_t* view);
        void send(Level level, std::string topic, std::string message) override;
    private:
        rd_view_t* view;
};
} // namespace logger