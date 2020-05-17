#include "score_tracker.hpp"

#include <iostream>
#include <thread>
#include <chrono>

namespace hh
{
    namespace score_tracker
    {
        uint16_t current_score = 0u;

        void display()
        {
            std::cout << "\nScore: " << std::dec << current_score << "/" << max_score << '\n';
        }

        void handle_win()
        {
            std::cout << "\nCongratulations!! You Win!! Restarting in 5 seconds...\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            current_score = 0; // Reset the score
        }
    } // namespace score_tracker
} // namespace hh