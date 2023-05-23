/**
 * @file splitter.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file for matrix splitter.
 * @date 04.05.2023
 */

#include <vector>
#include <cstdint>
#include <stdexcept>
#include "matrix.hpp"

#pragma once

/**
 * @brief A namespace for matrix splitter. It can split matrix into smaller blocks or merges them back.
 */
namespace splitter {
    /**
     * @brief Splits matrix into a vector of blocks of a specified width.
     *
     * @param data A matrix to be splitted.
     * @param block_width A block width.
     * @return std::vector<matrix<uint8_t>> Output vector of blocks.
     */
    std::vector<matrix<uint8_t>> split(matrix<uint8_t> data, uint8_t block_width);

    /**
    * @brief Merges a vector of blocks into one original matrix.
    *
    * @param data Input vector of blocks.
    * @param output_width Output matrix width.
    * @return matrix<uint8_t> Output matrix.
    */
    matrix<uint8_t> merge(std::vector<matrix<uint8_t>> data, uint16_t output_width);
};
