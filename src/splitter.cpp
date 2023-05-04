#include "splitter.hpp"


std::vector<matrix<uint8_t>> splitter::split(matrix<uint8_t> data, uint8_t block_width) {

    std::vector<matrix<uint8_t>> out_data;



    for (std::size_t master_i = 0; master_i < data.size(); master_i += block_width) {
        for (std::size_t master_j = 0; master_j < data[master_i].size(); master_j += block_width) {
            matrix<uint8_t> block;
            block.reserve(block_width);
            for (std::size_t i = 0; i < block_width && (master_i + i) < data.size(); ++i) {
                std::vector<uint8_t> row;
                row.reserve(block_width);
                for (std::size_t j = 0; j < block_width && (master_j + j) < data[master_i].size(); ++j) {
                    row.push_back(data[master_i + i][master_j + j]);
                }
                block.push_back(row);
            }
            out_data.push_back(block);
        }
    }

    return out_data;
}

matrix<uint8_t> splitter::merge(std::vector<matrix<uint8_t>> data, uint16_t output_width) {

    // printf("OUTPUT WIDTH = %u\n", output_width);
    // for (std::size_t i = 0; i < data.size(); ++i) {
    //     printf("%lu x %lu\n", data[i].size(), data[i][0].size());
    // }

    // Calculate data size
    std::size_t data_size = 0;
    for (std::size_t i = 0; i < data.size(); ++i) {
        data_size += data[i].size() * data[i][0].size();
    }
    std::size_t data_height = data_size / output_width;

    // Create out matrix
    matrix<uint8_t> out_data(data_height);
    for (std::size_t i = 0; i < data_height; ++i) {
        out_data[i].resize(output_width);
    }

    // Get blocks parameters
    std::size_t normal_block_height = data[0].size();
    std::size_t normal_block_width = data[0][0].size();
    std::size_t blocks_on_row = output_width / normal_block_width;
    if (output_width % normal_block_width != 0) {
        blocks_on_row++;
    }

    // Merge data
    std::size_t main_i = 0;
    std::size_t main_j = 0;
    for (std::size_t k = 0; k < data.size(); k += blocks_on_row) {
        for (std::size_t kk = 0; kk < blocks_on_row; ++kk) {
            for (std::size_t i = 0; i < data[k+kk].size(); ++i) {
                for (std::size_t j = 0; j < data[k+kk][i].size(); ++j) {
                    out_data[main_i + i][main_j + j] = data[k+kk][i][j];
                }
            }
            main_j += normal_block_width;
        }
        main_j = 0;
        main_i += normal_block_height;
    }

    return out_data;
}
