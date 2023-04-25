#include <vector>
#include <cstdint>
#include <queue>
#include <algorithm>
#include <map>
#include "matrix.hpp"
#include "tree.hpp"
#include "bitdata.hpp"

#pragma once

class Huffman {
    Tree canon_tree;
    std::map<int16_t, std::vector<bool>> code_book;
    std::vector<int8_t> code_lengths;
    std::vector<int16_t> code_letters;
    uint16_t max_code_len;

    static uint8_t bits_to_uint8(bitdata::bits::iterator data) {
        uint8_t out = 0;
        for (int i = 0; i < 8; ++i) {
            out = out | static_cast<uint8_t>(*data);
            data++;
            out = out << 1;
        }
        return out;
    }

    static int16_t bits_to_int16(bitdata::bits::iterator data) {
        int16_t out = 0;
        for (int i = 0; i < 16; ++i) {
            out = out | static_cast<int8_t>(*data);
            data++;
            out = out << 1;
        }
        return out;
    }

    static std::map<int16_t, unsigned> get_alphabet_counts(std::vector<int16_t> data);
    void generate_code_book(std::vector<int16_t> data);
    void construct_tree();

    public:

        std::vector<bool> code(std::vector<int16_t> data);
        std::vector<int16_t> decode(bitdata::bits::iterator reading_head);

};