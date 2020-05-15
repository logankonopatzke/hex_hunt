#pragma once

#include <inttypes.h>

#include "general_config.hpp"

namespace hh
{
    namespace score_tracker
    {
        constexpr auto max_score = NUMBER_OF_HEX_ENTITIES * SCORE_INCREMENT_PER_CAPTURE;
        extern uint16_t current_score; // This should be of enough size to hold the score

        void display();
    } // namespace score_tracker
} // namespace hh