#include "score_tracker.hpp"

#include <iostream>

namespace hh
{
    namespace score_tracker
    {
        uint16_t current_score = 0u;

        void display()
        {
            std::cout << "\nScore: " << current_score << "/" << max_score << '\n';
        }
    } // namespace score_tracker
} // namespace hh