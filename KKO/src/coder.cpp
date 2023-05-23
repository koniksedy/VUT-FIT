/**
 * @file coder.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief An implementation of a general coder.
 * @date 04.05.2023
 */

#include "coder.hpp"


#define BLOCK_WIDTH 64      // Width of data block during adaptive coding

/**
 * @brief Appends data from bit vector at the end of output data using move operation.
 *
 * @param bit_data_raw Bit vector to be moved.
 */
inline void Coder::move_back_to_output(bitdata::bits bit_data_raw) {
    this->data_out.insert(this->data_out.end(), std::make_move_iterator(bit_data_raw.begin()), std::make_move_iterator(bit_data_raw.end()));
}

/**
 * @brief Loads raw data from file.
 *
 * @param input File name.
 * @param width Width of coded image.
 * @return true: Successful load from file.
 * @return false: Unsuccessful load from file.
 */
bool Coder::load(char *input, uint16_t width) {
    std::ifstream stream(input, std::ios::in | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Input file not found." << std::endl;
        return false;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(stream)),
                              (std::istreambuf_iterator<char>()));

    this->width = width;
    std::size_t height = data.size() / width;

    this->data_in.reserve(height);
    for (std::size_t i = 0; i < height; ++i) {
        std::vector<uint8_t> row(width);
        row.reserve(width);
        std::size_t last = std::min((i * width) + width, data.size());
        move(data.begin() + (i * width), data.begin() + last, row.begin());
        this->data_in.push_back(row);
    }

    return true;
}

/**
 * @brief Codes loaded data.
 */
void Coder::run() {
    // CODE width (header)
    this->move_back_to_output(bitdata::to_bits(this->width, 16));

    // CODE adaptive coding (header)
    std::vector<matrix<uint8_t>> blocks;
    if (this->adaptive) {
        this->move_back_to_output(bitdata::to_bits(static_cast<uint8_t>(1), 1));
        blocks = splitter::split(this->data_in, BLOCK_WIDTH);
    } else {
        this->move_back_to_output(bitdata::to_bits(static_cast<uint8_t>(0), 1));
        blocks.push_back(this->data_in);
    }

    // CODE model mode (header)
    if (this->model) {
        this->move_back_to_output(bitdata::to_bits(static_cast<uint8_t>(1), 1));
    } else {
        this->move_back_to_output(bitdata::to_bits(static_cast<uint8_t>(0), 1));
    }

    // Get models
    std::vector<Model> models;
    models.reserve(blocks.size());
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        if (this->model) {
            Model left_right(blocks[i], Model::PassThru::left_right);
            Model top_down(blocks[i], Model::PassThru::top_down);
            Model main_diagonal(blocks[i], Model::PassThru::main_diagonal);
            Model side_diagonal(blocks[i], Model::PassThru::side_diagonal);
            models.push_back(std::min({left_right, top_down, main_diagonal, side_diagonal}));
        } else {
            models.push_back(Model(blocks[i]));
        }
    }

    // CODE models (header) + huffman (data)
    for (std::size_t i = 0; i < models.size(); ++i) {
        // CODE pass (header) + master point (header)
        this->move_back_to_output(bitdata::to_bits(static_cast<uint8_t>(models[i].get_pass()), 2));
        this->move_back_to_output(bitdata::to_bits(models[i].get_master_point(), 8));

        // CODE huffman (data)
        Huffman huffman = Huffman();
        this->move_back_to_output(huffman.code(linearizer::linearize(models[i].get_data())));
    }
}

/**
 * @brief Saves coded data into the output file.
 *
 * @param output File name.
 * @return true: Successful save.
 * @return false: Unsuccessful save.
 */
bool Coder::save(char *output) {

    std::ofstream stream(output, std::ios::out | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Output file was not created successfully." << std::endl;
        return false;
    }

    std::size_t i = 0;
    uint8_t ch;
    uint8_t d;
    while (i < this->data_out.size()) {
        ch = 0;
        // Create padding if necessary.
        for (std::size_t j = 0; j < 8; ++j) {
            ch = ch << 1;
            if (i < this->data_out.size()) {
                d = this->data_out[i++];
                ch = ch | d;
            }
        }
        stream.write(reinterpret_cast<char*>(&ch), 1);
    }
    return true;
}
