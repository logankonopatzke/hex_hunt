#pragma once

#include <utility>
#include <vector>
#include <inttypes.h>

#include "general_config.hpp"

namespace hh
{
    class cell_table
    {
    public:
        cell_table();

        cell_table(const std::pair<uint8_t, uint8_t> &size);

        ~cell_table();

        void print();

        void resize(const std::pair<uint8_t, uint8_t> &size);

        void add_hex_entity(const std::pair<uint8_t, uint8_t> &pos, uint8_t val);

        std::pair<size_t, size_t> size();

        bool is_cell_occupied(const std::pair<uint8_t, uint8_t> &pos);

        bool is_cell_in_range(const std::pair<uint8_t, uint8_t> &pos);

        uint8_t &get_cell_value(const std::pair<uint8_t, uint8_t> &pos); // We can modify the value at the selected position because we return a reference

        void handle_capture();

        void handle_movement();

    private:
        std::vector<std::vector<uint8_t>> table; // Rows x Columns
    public:
        std::pair<uint8_t, uint8_t> cursor_position{DEFAULT_CURSOR_X, DEFAULT_CURSOR_Y}; // Let's setup our default position
        uint8_t cursor_value = 0;
        std::vector<uint8_t> next_cursor_values;
    };
} // namespace hh