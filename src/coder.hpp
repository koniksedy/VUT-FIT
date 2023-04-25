#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "matrix.hpp"

#pragma once

class Coder {
    bool model = false;
    bool adaptive = false;
    uint16_t width = 0;
    uint16_t height = 0;    // ????
    matrix<uint8_t> data_in;
    std::vector<bool> data_out;

    public:
        void set_model(bool is_model) {
            this->model = is_model;
        }

        void set_adaptive(bool is_adaptive) {
            this->adaptive = is_adaptive;
        }

        bool load(char *input, uint16_t width);
        void run();
        bool save(char *output);
};