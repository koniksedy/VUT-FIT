#include "linearizer.hpp"


std::vector<std::vector<int16_t>> linearizer::linearize(std::vector<matrix<int16_t>> data) {

    std::vector<std::vector<int16_t>> out(data.size());

    for (std::size_t k = 0; k < data.size(); ++k) {
        out[k].reserve(data[k].size() * data[k][0].size());

        for (std::size_t i = 0; i < data[k].size(); ++i) {
            for (std::size_t j = 0; j < data[k][i].size(); ++j) {
                out[k].push_back(data[k][i][j]);
            }
        }
    }

    return out;
}

std::vector<matrix<int16_t>> linearizer::delinearize(std::vector<std::vector<int16_t>> data, uint16_t width) {

    std::vector<matrix<int16_t>> out;

    std::size_t normal_block_width = std::sqrt(data[0].size());
    std::size_t small_block_width = width % normal_block_width;
    std::size_t blocks_in_row = width / normal_block_width;
    if (small_block_width != 0) {
        blocks_in_row++;
    }

    std::size_t block_width;
    std::size_t block_height;
    std::size_t main_i;

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
