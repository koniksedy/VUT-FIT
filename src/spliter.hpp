#include <vector>
#include <cstdint>
#include <stdexcept>
#include "matrix.hpp"

#pragma once


struct Splitter {

        template<typename T>
        static std::vector<matrix<T>> split(matrix<T> data, uint8_t block_width);

        template<typename T>
        static matrix<T> merge(std::vector<std::vector<T>> data, uint16_t output_width);
};
