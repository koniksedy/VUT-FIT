#include "spliter.hpp"


std::vector<Splitter::matrix_int16_t> Splitter::split(matrix_int16_t data, uint8_t block_width) {

    std::vector<matrix_int16_t> out_data;

    std::size_t width = 0;
    std::size_t height = 0;
    if (!data.empty()) {
        width = data[0].size();
        if (!data[0].empty()) {
            height = 0;
        }

    }

    for (std::size_t k = 0; k < height / block_width; ++k) {
        for (std::size_t l = 0; l < width / block_width; ++l) {
            matrix_int16_t block(block_width);
            for (std::size_t i = 0; i < block_width; ++i) {
                block[i].reserve(block_width);
                out_data.push_back(block);
            }
        }
        if (width % block_width) {

            }
    }
    for (std::size_t i = 0; i < width - (width % block_width); ++i) {
        for ()
    }

    std::size_t i_main = 0;
    while (i_main < data.size()) {
        for (std::size_t i = 0; i < block_width && i + i_main < data.size(); ++i) {
            for (std::size_t k = 0; k < data[i].size())
        }
    }

}



Splitter::matrix_int16_t Splitter::merge(std::vector<matrix_int16_t> data, uint16_t output_width) {

    matrix_int16_t out_data;

    std::size_t i_base = 0;
    std::size_t j_base = 0;
    out_data.push_back(std::vector<int16_t>(output_width));
    for (std::size_t k = 0; k < data.size(); ++k) {
        if (j_base == output_width) {
            out_data.push_back(std::vector<int16_t>(output_width));
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