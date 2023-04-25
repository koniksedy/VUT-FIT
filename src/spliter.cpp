#include "spliter.hpp"


template<typename T>
std::vector<matrix<T>> Splitter::split(matrix<T> data, uint8_t block_width) {

    std::vector<matrix<T>> out_data;

    std::size_t width = 0;
    std::size_t height = 0;
    if (!data.empty()) {
        width = data[0].size();
        if (!data[0].empty()) {
            height = data[0][0].size();
        }
    }

    std::size_t h = 0;
    std::size_t block_height = block_width;
    while (h != height) {
        if (h + block_height > height) {
            block_height = height % block_height;
            h = height;
        } else {
            h += block_height;
        }
        for (std::size_t l = 0; l < width / block_width; ++l) {
           matrix<T> block(block_height);
            for (std::size_t i = 0; i < block_width; ++i) {
                block[i].reserve(block_width);
                out_data.push_back(block);
            }
        }
        std::size_t rest_w = width % block_width;
        if (rest_w != 0) {
            matrix<T> block(block_height);
            for (std::size_t i = 0; i < rest_w; ++i) {
                block[i].reserve(rest_w);
                out_data.push_back(block);
            }
        }
    }
}

template<typename T>
matrix<T> Splitter::merge(std::vector<std::vector<T>> data, uint16_t output_width) {

    matrix<T> out_data;

    std::size_t i_base = 0;
    std::size_t j_base = 0;
    out_data.push_back(std::vector<T>(output_height));
    for (std::size_t k = 0; k < data.size(); ++k) {
        if (j_base == output_width) {
            out_data.push_back(std::vector<T>(output_width));
            i_base++;
            j_base = 0;
        }
        for (std::size_t i = 0; i < data[k].size(); ++i) {
            for (std::size_t j = 0; j < data[k][j].size(); ++j) {
                out_data[i_base + i][j_base + j] = data[k][j][i];
            }
        }
        if (!data[k].empty()) {
            j_base += data[k].size();
        }
    }
}
