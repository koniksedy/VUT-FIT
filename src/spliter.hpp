#include <vector>
#include <cstdint>
#include <stdexcept>
#include "matrix.hpp"

#pragma once


namespace splitter {

        std::vector<matrix<uint8_t>> split(matrix<uint8_t> data, uint8_t block_width);

        matrix<int16_t> merge(std::vector<matrix<int16_t>> data, uint16_t output_width);
};
