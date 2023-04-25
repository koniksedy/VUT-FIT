#include "linearizer.hpp"


template<typename T>
std::vector<std::vector<T>> Linearizer::linearize(std::vector<matrix<T>> data) {

    std::vector<std::vector<T>> out(data.size());
    std::size_t matrix_size;

    for (std::size_t k = 0; k < data.size(); ++k) {
        if (!data[k].empty() && !data[k][0].empty()) {
            matrix_size = data[k].size() * data[k][0].size();
            out.push_back(std::vector<T>(matrix_size));
        }
        for (std::size_t i = 0; i < data[k].size(); ++i) {
            for (std::size_t j = 0; j < data[k][i].size(); ++j) {
                out[k][i*data[k].size() + j] = data[k][i][j];
            }
        }
    }

    return out;
}

template<typename T>
std::vector<matrix<T>> Linearizer::delinearize(std::vector<std::vector<T>> data, uint16_t width) {

    std::vector<matrix<T>> out;

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
        if (blocks_in_row % (k+1) == 0) {
            block_width = small_block_width;
        } else {
            block_height = normal_block_width;
        }
        block_height = data[k].size() / block_width;
        out.push_back(matrix<T>(block_height));
        for (std::size_t i = 0; i < block_height; ++i) {
            out[k].push_back(std::vector<T>(block_width));
            for (std::size_t j = 0; j < block_width; ++j) {
                out[k][i][j] = data[k][main_i++];
            }
        }
    }

    return out;
}
