# include "coder.hpp"


bool Coder::load(char *input, uint16_t width) {
    std::ifstream stream(input, std::ios::binary);

    if (!stream.good()) {
        std::cout << "Input file not found." << std::endl;
        return false;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(stream)),
                              (std::istreambuf_iterator<char>()));

    this->width = width;
    this->height = data.size() / width;
    std::cout << data.size() << std::endl;

    this->data_in.reserve(this->height);
    for (size_t i = 0; i < this->height; ++i) {
        std::vector<uint8_t> row(width);
        row.reserve(width);
        size_t last = std::min((i * width) + width, data.size());
        move(data.begin() + (i * width), data.begin() + last, row.begin());
        this->data_in.push_back(row);
    }

    return true;
}


void Coder::run(char *output) {

    for (uint16_t i = 0; i < this->height; ++i) {
        std::cout << i << ": ";
        for (uint8_t d: this->data_in[i]) {
            printf("%x ", d);
        }
        std::cout << std::endl;
    }
}
