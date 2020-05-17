#include "cell_table.hpp"

#include <iostream>
#include <algorithm>
#include <iomanip>

#include "general_config.hpp"
#include "cell_movement.hpp"
#include "score_tracker.hpp"

namespace hh
{
    cell_table::cell_table()
    {
        resize({DEFAULT_TABLE_SIZE, DEFAULT_TABLE_SIZE});
    }

    cell_table::cell_table(const std::pair<uint8_t, uint8_t> &size)
    {
        resize(size);
    }

    cell_table::~cell_table()
    {
    }

    void cell_table::print()
    {
        auto print_horizontal_border = [](size_t size) -> void {
            static const size_t border_length = (size * 8) - (size / 2);
            for (size_t i = 0; i < border_length; ++i)
                std::cout << '_';
        };

        static const auto border_size = table.at(0).size();
        print_horizontal_border(border_size);

        std::cout << "\n\n";

        for (size_t i = 0; i < table.size(); ++i)
        {
            for (size_t j = 0; j < table.at(i).size(); ++j)
            {
                const int value = table.at(i).at(j);
                const bool is_cursor = (cursor_position.first == j && cursor_position.second == i);

                if (value && !is_cursor)
                    std::cout << " | 0x" << std::hex << value;
                else if (!is_cursor)
                    std::cout << " | 0x00";
                else
                    std::cout << " | 0" << std::dec << static_cast<int>(cursor_value); // Test
            }

            std::cout << " |\n";
        }

        print_horizontal_border(border_size);
    }

    void cell_table::resize(const std::pair<uint8_t, uint8_t> &size)
    {
        table.resize(size.second);

        for (size_t i = 0; i < table.size(); ++i)
        {
            table.at(i).resize(size.first);

            std::generate(table.at(i).begin(), table.at(i).end(), []() { return static_cast<uint8_t>(0); });
        }
    }

    void cell_table::add_hex_entity(const std::pair<uint8_t, uint8_t> &pos, uint8_t val)
    {
        if (!is_cell_occupied(pos))
            table.at(pos.second).at(pos.first) = val;
    }

    std::pair<size_t, size_t> cell_table::size()
    {
        return {table.size(), table.at(0).size()};
    }

    bool cell_table::is_cell_occupied(const std::pair<uint8_t, uint8_t> &pos)
    {
        return table.at(pos.second).at(pos.first) != 0;
    }

    bool cell_table::is_cell_in_range(const std::pair<uint8_t, uint8_t> &pos)
    {
        return (pos.first >= 0 && pos.first <= size().first - 1)        // Verify in range on x axis
               && (pos.second >= 0 && pos.second <= size().second - 1); // Verify in range on y axis
    }

    uint8_t &cell_table::get_cell_value(const std::pair<uint8_t, uint8_t> &pos)
    {
        return table.at(pos.second).at(pos.first);
    }

    void cell_table::handle_capture()
    {
        if (next_cursor_values.size() > 1) // The game should end on the next tick due to the max score being reached
        {
            next_cursor_values.pop_back();            // Eliminate previously used value
            cursor_value = next_cursor_values.back(); // Update cursor value
        }
        score_tracker::current_score += SCORE_INCREMENT_PER_CAPTURE; // Increase score
        get_cell_value({cursor_position}) = 0;                       // Purge the hex value from the table
        // We can simply use the position of our cursor because we have captured the hex entity
    }

    void cell_table::handle_movement()
    {
        if (cell_movement::move_up())
        {
            const std::pair<uint8_t, uint8_t> next_cursor_position{cursor_position.first, cursor_position.second - 1};
            if (!is_cell_in_range(next_cursor_position))
                return;

            if (is_cell_occupied(next_cursor_position) && get_cell_value(next_cursor_position) == cursor_value) // If we attempted a capture and the capture is valid
            {
                cursor_position = next_cursor_position; // Update cursor position
                handle_capture();                       // Successful capture by user
            }
            else if (!is_cell_occupied(next_cursor_position))
                cursor_position = next_cursor_position;
        }
        else if (cell_movement::move_down())
        {
            const std::pair<uint8_t, uint8_t> next_cursor_position{cursor_position.first, cursor_position.second + 1};
            if (!is_cell_in_range(next_cursor_position))
                return;

            if (is_cell_occupied(next_cursor_position) && get_cell_value(next_cursor_position) == cursor_value)
            {
                cursor_position = next_cursor_position;
                handle_capture();
            }
            else if (!is_cell_occupied(next_cursor_position))
                cursor_position = next_cursor_position;
        }
        else if (cell_movement::move_left())
        {
            const std::pair<uint8_t, uint8_t> next_cursor_position{cursor_position.first - 1, cursor_position.second};
            if (!is_cell_in_range(next_cursor_position))
                return;

            if (is_cell_occupied(next_cursor_position) && get_cell_value(next_cursor_position) == cursor_value)
            {
                cursor_position = next_cursor_position;
                handle_capture();
            }
            else if (!is_cell_occupied(next_cursor_position))
                cursor_position = next_cursor_position;
        }
        else if (cell_movement::move_right())
        {
            const std::pair<uint8_t, uint8_t> next_cursor_position{cursor_position.first + 1, cursor_position.second};
            if (!is_cell_in_range(next_cursor_position))
                return;

            if (is_cell_occupied(next_cursor_position) && get_cell_value(next_cursor_position) == cursor_value)
            {
                cursor_position = next_cursor_position;
                handle_capture();
            }
            else if (!is_cell_occupied(next_cursor_position))
                cursor_position = next_cursor_position;
        }
    }

} // namespace hh