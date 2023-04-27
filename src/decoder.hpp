#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "bitdata.hpp"
#include "model.hpp"
#include "huffman.hpp"
#include "linearizer.hpp"
#include "splitter.hpp"

#pragma once


class Decoder {
    bool model = false;
    bool adaptive = false;
    uint16_t width = 0;
    bitdata::bits data_in;
    matrix<uint8_t> data_out;

    bool is_near_end(bitdata::bits::iterator iter, std::size_t distance);

    public:
        bool load(char *input);
        void run();
        bool save(char *output);
};
