#include "decoder.hpp"



bool Decoder::is_near_end(bitdata::bits::iterator iter, std::size_t distance) {
    for (std::size_t i = 0; i < distance; ++i, ++iter) {
        if (iter == this->data_in.end()) {
            return true;
        }
    }
    return false;
}

bool Decoder::load(char *input) {
    std::ifstream stream(input, std::ios::in | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Input file not found." << std::endl;
        return false;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(stream)),
                              (std::istreambuf_iterator<char>()));

    this->data_in.reserve(8 * data.size());
    bitdata::bits data_raw;
    for (size_t i = 0; i < data.size(); ++i) {
        data_raw = bitdata::to_bits(data[i], 8);
        this->data_in.insert(this->data_in.end(), data_raw.begin(), data_raw.end());
    }

    return true;
}


void Decoder::run() {

    bitdata::bits::iterator reading_head = this->data_in.begin();

    // DECODE width
    this->width = bitdata::bits_to_uint16(reading_head, 16);
    reading_head += 16;

    // DECODE adaptive mode
    this->adaptive = static_cast<bool>(bitdata::bits_to_uint8(reading_head, 1));
    reading_head++;

    // DECODE model mode
    this->model = static_cast<bool>(bitdata::bits_to_uint8(reading_head, 1));
    reading_head++;

    // DECODE model data
    std::vector<Model::PassThru> models_pass;
    std::vector<uint8_t> models_master_point;
    std::vector<std::vector<int16_t>> models_linear_data;
    while (!this->is_near_end(reading_head, 16))
    {
        // DECODE model pass
        models_pass.push_back(static_cast<Model::PassThru>(bitdata::bits_to_uint8(reading_head, 2)));
        reading_head += 2;

        // DECODE model master point
        models_master_point.push_back(bitdata::bits_to_uint8(reading_head, 8));
        reading_head += 8;

        // for (bitdata::bits::iterator kokot = reading_head; reading_head != this->data_in.end(); ++kokot) {
        //     std::cout << (*kokot ? "1" : "0");
        // }
        // std::cout << std::endl;

        // DECODE mode linear data
        Huffman huffman = Huffman();
        models_linear_data.push_back(huffman.decode(reading_head));
    }
    std::vector<matrix<int16_t>> models_matrix_data = linearizer::delinearize(models_linear_data, this->width);

    // Constructing data blocks
    std::vector<matrix<uint8_t>> blocks_data;
    blocks_data.reserve(models_matrix_data.size());
    for (std::size_t i = 0; i < models_master_point.size(); ++i) {
        if (this->model) {
            Model model(models_matrix_data[i], models_master_point[i], models_pass[i]);
            blocks_data.push_back(model.reconstruct());
        } else {
            matrix<uint8_t> model_data(models_matrix_data[i].size());
            for (std::size_t ii = 0; ii < models_matrix_data[i].size(); ++ii) {
                model_data[ii].reserve(models_matrix_data[i][ii].size());
                for (std::size_t jj = 0; jj < models_matrix_data[i][ii].size(); ++jj) {
                    model_data[ii].push_back(static_cast<uint8_t>(models_matrix_data[i][ii][jj]));
                }
            }
            blocks_data.push_back(model_data);
        }
    }

    // Merging data blocks
    this->data_out = splitter::merge(blocks_data, this->width);
}

bool Decoder::save(char *output) {
    std::ofstream stream(output, std::ios::out | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Output file was not created successfully." << std::endl;
        return false;
    }

    for (std::size_t i = 0; i < this->data_out.size(); ++i) {
        for (std::size_t j = 0; j < this->data_out[i].size(); ++j) {
            stream.write(reinterpret_cast<char*>(&this->data_out[i][j]), 1);
        }
    }

    return true;
}
