#include "core.hpp"

#include <random>
#include <iostream>
#include <thread>
#include <chrono>

#include "cell_table.hpp"
#include "cell_movement.hpp"
#include "score_tracker.hpp"

namespace hh
{
    namespace core
    {
        static int random_number(const std::pair<int, int> &min_max)
        {
            std::random_device device;
            std::mt19937 generator(device());
            std::uniform_int_distribution<> distributor(min_max.first, min_max.second);

            return distributor(generator);
        }

        static void generate_entities(cell_table &table)
        {
            for (uint8_t i = 0; i < NUMBER_OF_HEX_ENTITIES; ++i)
            {
                std::pair<uint8_t, uint8_t> random_position;
                do
                {
                    const std::pair<uint8_t, uint8_t> position_range{table.size().first - 1, table.size().second - 1};
                    random_position = {random_number({0, position_range.first}), random_number({0, position_range.second})};

                } while (table.is_cell_occupied(random_position) &&                                                 // Make sure the entity we are spawning is not ontop of the cursor or another entity
                         (random_position.first - table.cursor_position.first >= MIN_HEX_ENTITY_SPAWN_DISTANCE) &&  // Make sure the entity is far enough away
                         (random_position.second - table.cursor_position.second >= MIN_HEX_ENTITY_SPAWN_DISTANCE)); // ^

                const auto value = random_number({1, 255});   // We don't want to start at 0 to differentiate between hex entities and open spaces
                table.add_hex_entity(random_position, value); // ^ Max range of unsigned 1 byte value
                table.next_cursor_values.push_back(value);
            }

            table.cursor_value = table.next_cursor_values.back();
        }

        static void tick(cell_table &table)
        {
            auto refresh_table = [&table]() -> void {
                system("clear");
                score_tracker::display();
                table.print();
            };

            std::this_thread::sleep_for(std::chrono::milliseconds(80)); // This is mostly necessary for the input handling
            // We don't need to be polling inputs every millisecond

            table.handle_movement();

            static bool first_run = true;
            if (cell_movement::move_any() || first_run)
            {
                first_run = false;

                if (score_tracker::current_score == score_tracker::max_score) // If the the user has captured all the hex entities
                {
                    refresh_table();                                              // Refresh the table to prepare for next round
                    score_tracker::handle_win();                                  // Display win to the user and handle necessary modification to score
                    table.cursor_position = {DEFAULT_CURSOR_X, DEFAULT_CURSOR_Y}; // Reset cursor position
                    generate_entities(table);                                     // Regenerate entities to allow the user to keep playing
                }

                refresh_table(); // Update table to reflect movement input
            }
        }

        void setup()
        {
            std::cout << R"(
██╗░░██╗███████╗██╗░░██╗  ██╗░░██╗██╗░░░██╗███╗░░██╗████████╗
██║░░██║██╔════╝╚██╗██╔╝  ██║░░██║██║░░░██║████╗░██║╚══██╔══╝
███████║█████╗░░░╚███╔╝░  ███████║██║░░░██║██╔██╗██║░░░██║░░░
██╔══██║██╔══╝░░░██╔██╗░  ██╔══██║██║░░░██║██║╚████║░░░██║░░░
██║░░██║███████╗██╔╝╚██╗  ██║░░██║╚██████╔╝██║░╚███║░░░██║░░░
╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝  ╚═╝░░╚═╝░╚═════╝░╚═╝░░╚══╝░░░╚═╝░░░
    )";

            std::cout << "\nGame starting in 5 seconds...\n";

            std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait a bit before continuing into the game

            cell_table table({8, 8}); // Setup our table object with size 8x8

            generate_entities(table); // Initial entity generation

            while (true)
            {
                tick(table); // Endless run until user exits
            }
        }
    } // namespace core
} // namespace hh