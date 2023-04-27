#include <vector>
#include <tuple>
#include <cmath>
#include <cstdint>
#include "matrix.hpp"

#pragma once


namespace linearizer {

    std::vector<int16_t> linearize(matrix<int16_t> data);

    std::vector<matrix<int16_t>> delinearize(std::vector<std::vector<int16_t>> data,  uint16_t width);

};
