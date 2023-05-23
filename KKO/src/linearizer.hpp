/**
 * @file linearizer.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file for the linearizer.
 * @date 04.05.2023
 */

#include <vector>
#include <tuple>
#include <cmath>
#include <cstdint>
#include "matrix.hpp"

#pragma once


/**
 * @brief Namespace of linearizer functions.
 */
namespace linearizer {
    /**
     * @brief Transforms matrix into vector.
     *
     * @param data Input matrix.
     * @return std::vector<int16_t> Output vector.
     */
    std::vector<int16_t> linearize(matrix<int16_t> data);

    /**
     * @brief Transforms a vector of vectors into a vector of matrices based on the width of an output image.
     *
     * @param data Input vector of vectors.
     * @param width A width of an output image.
     * @return std::vector<matrix<int16_t>> Output vector of matrices.
     */
    std::vector<matrix<int16_t>> delinearize(std::vector<std::vector<int16_t>> data,  uint16_t width);

};
