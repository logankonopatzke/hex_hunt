/*
1. Console window table layout (each cell must have 6 spaces (4 for the hex number: 0xAB))
_______________
_______________
| | | | | | | |
---------------
| | | | | | | |
---------------
| | | | | | | |
_______________

2. We should have an introduction/welcome screen on startup (big bubble letters)

3. Devise a method of cell navigation via a helper function

4. Implement a cross platform input system that abstracts basic inputs down to a class we can manage

5. ???
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <string>

#include "cell_table.hpp"
#include "cell_movement.hpp"
#include "general_config.hpp"
#include "score_tracker.hpp"

namespace hh
{
    static int random_number(const std::pair<int, int> &min_max)
    {
        std::random_device device;
        std::mt19937 generator(device());
        std::uniform_int_distribution<> distributor(min_max.first, min_max.second);

        return distributor(generator);
    }

    static void generate_entities(hh::cell_table &table)
    {
        for (uint8_t i = 0; i < NUMBER_OF_HEX_ENTITIES; ++i)
        {
            std::pair<uint8_t, uint8_t> random_position;
            do
            {
                const std::pair<uint8_t, uint8_t> position_range{table.size().first - 1, table.size().second - 1};
                random_position = {random_number({0, position_range.first}), random_number({0, position_range.second})};

            } while (table.is_cell_occupied(random_position) &&                                                // Make sure the entity we are spawning is not ontop of the cursor or another entity
                     (random_position.first - table.cursor_position.first >= MIN_HEX_ENTITY_SPAWN_DISTANCE) && // Make sure the entity is far enough away
                     (random_position.second - table.cursor_position.second >= MIN_HEX_ENTITY_SPAWN_DISTANCE));

            const auto t = random_number({1, 255});
            table.cursor_value = t;
            table.add_hex_entity(random_position, t); // Max range of unsigned 1 byte value
        }
    }
} // namespace hh

int main()
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

    hh::cell_table table({8, 8}); // Setup our table object with size 8x8

    hh::generate_entities(table); // Initial entity generation

    auto refresh_table = [&table]() -> void {
        system("clear");
        hh::score_tracker::display();
        table.print();
    };

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(80)); // This is mostly necessary for the input handling
        // We don't need to be polling inputs every millisecond

        table.handle_movement();

        static bool first_run = true;
        if (hh::cell_movement::move_any() || first_run)
        {
            first_run = false;

            if (hh::score_tracker::current_score == hh::score_tracker::max_score) // If the the user has captured all the hex entities
            {
                hh::generate_entities(table); // Regenerate entities to allow the user to keep playing
                refresh_table();
            }

            refresh_table(); // Update table to reflect movement input
        }
    }

    std::cin.get();

    return 0;
}