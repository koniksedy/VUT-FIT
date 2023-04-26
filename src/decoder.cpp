#include "decoder.hpp"


bool Decoder::load(char *input) {
    std::ifstream stream(input, std::ios::in | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Input file not found." << std::endl;
        return false;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(stream)),
                              (std::istreambuf_iterator<char>()));

    this->data_in.reserve(8 * data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        uint8_t ch = data[i];
        uint8_t mask = 0x80;
        for (size_t j = 0; j < 8; ++j) {
            this->data_in.push_back(ch & mask);
            mask = mask >> 1;
        }
    }

    return true;
}


void Decoder::run() {

    // Test - load
    std::cout << "Loaded" << std::endl;
    assert(this->data_in.size() % 8 == 0);
    unsigned j = 0;
    bitdata::bits::iterator reading_head = this->data_in.begin();
    while (reading_head != this->data_in.end()) {
        printf("%x ", bitdata::bits_to_uint8(reading_head, 8));
        reading_head += 8;
        j += 1;
        if (j == 8) {
            std::cout << std::endl;
            j = 0;
        }
    }


}

bool Decoder::save(char *output) {
    std::ofstream stream(output, std::ios::out | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Output file was not created successfully." << std::endl;
        return false;
    }

    stream.write(reinterpret_cast<char*>(this->data_out.data()), this->data_out.size());

    return true;
}
