#include <vector>
#include <tuple>
#include <cmath>
#include <cstdint>
#include "matrix.hpp"

#pragma once


struct Linearizer {

    template<typename T>
    static std::vector<std::vector<T>> linearize(std::vector<matrix<T>> data);

    template<typename T>
    static std::vector<matrix<T>> delinearize(std::vector<std::vector<T>> data,  uint16_t width);

};