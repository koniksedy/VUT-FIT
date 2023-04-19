#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>


class Coder {
    using matrix_uint8_t = std::vector<std::vector<uint8_t>>;

    bool model = false;
    bool adaptive = false;
    uint16_t width = 0;
    uint16_t height = 0;
    matrix_uint8_t data_in;

    public:
        void set_model(bool is_model) {
            this->model = is_model;
        }

        void set_adaptive(bool is_adaptive) {
            this->adaptive = is_adaptive;
        }

        bool load(char *input, uint16_t width);
        void run(char *output);
};