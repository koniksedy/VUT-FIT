/**
 * @file linearizer.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief An implementation of linearizer. It can linearize matrix to vector and back.
 * @date 04.05.2023
 */

#include "linearizer.hpp"

#define BLOCK_WIDTH 64  // Width of data block during adaptive coding


/**
 * @brief Transforms matrix into vector.
 *
 * @param data Input matrix.
 * @return std::vector<int16_t> Output vector.
 */
std::vector<int16_t> linearizer::linearize(matrix<int16_t> data) {
    std::vector<int16_t> out;
    out.reserve(data.size() * data[0].size());

    for (std::size_t i = 0; i < data.size(); ++i) {
        for (std::size_t j = 0; j < data[i].size(); ++j) {
            out.push_back(data[i][j]);
        }
    }
    return out;
}

/**
 * @brief Transforms a vector of vectors into a vector of matrices based on the width of an output image.
 *
 * @param data Input vector of vectors.
 * @param width A width of an output image.
 * @return std::vector<matrix<int16_t>> Output vector of matrices.
 */
std::vector<matrix<int16_t>> linearizer::delinearize(std::vector<std::vector<int16_t>> data, uint16_t width) {
    std::vector<matrix<int16_t>> out;

    // Calculate block width
    std::size_t normal_block_width = (data.size() == 1 ? width : BLOCK_WIDTH);
    std::size_t small_block_width = width % normal_block_width;
    std::size_t blocks_in_row = width / normal_block_width;
    if (small_block_width != 0) {
        blocks_in_row++;
    }

    std::size_t block_width;
    std::size_t block_height;
    std::size_t main_i;

    // Transformation
    for (std::size_t k = 0; k < data.size(); ++k) {
        main_i = 0;
        if ((k + 1) % blocks_in_row == 0 && small_block_width != 0) {
            block_width = small_block_width;
        } else {
            block_width = normal_block_width;
        }
        block_height = data[k].size() / block_width;
        matrix<int16_t> block(block_height);
        out.push_back(block);
        for (std::size_t i = 0; i < block_height; ++i) {
            out[k][i].resize(block_width);
            for (std::size_t j = 0; j < block_width; ++j) {
                out[k][i][j] = data[k][main_i++];
            }
        }
    }

    return out;
}
