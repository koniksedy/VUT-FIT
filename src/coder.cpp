# include "coder.hpp"


bool Coder::load(char *input, uint16_t width) {
    std::ifstream stream(input, std::ios::in | std::ios::binary);

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


void Coder::run() {

}

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
        for (std::size_t j = 0; j < 8; ++j) {
            if (i < this->data_out.size()) {
                d = this->data_out[i++];
                ch = ch | d;
            }
            ch = ch << 1;
        }
        stream.write(reinterpret_cast<char*>(&ch), 1);
    }
    return true;
}
