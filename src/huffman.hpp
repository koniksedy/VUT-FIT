#include <map>
#include <queue>
#include <vector>
#include <cstdint>
#include <algorithm>
#include "tree.hpp"
#include "matrix.hpp"
#include "bitdata.hpp"

#pragma once


class Huffman {
    Tree canon_tree;
    std::map<int16_t, std::vector<bool>> code_book;
    std::vector<uint16_t> code_lengths;
    std::vector<int16_t> code_letters;
    uint16_t max_code_len;
    static const int16_t EOF = -256;

    static std::map<int16_t, unsigned> get_alphabet_counts(std::vector<int16_t> data);
    void generate_code_book(std::vector<int16_t> data);
    void construct_tree();

    public:

        std::vector<bool> code(std::vector<int16_t> data);
        std::vector<int16_t> decode(bitdata::bits::iterator reading_head);

};
