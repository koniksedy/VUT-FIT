/**
 * @file huffman.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file for Huffman coding and decoding.
 * @date 04.05.2023
 */

#include <map>
#include <queue>
#include <memory>
#include <vector>
#include <cstdint>
#include <algorithm>
#include "tree.hpp"
#include "matrix.hpp"
#include "bitdata.hpp"

#pragma once


/**
 * @brief A Huffman class for coding and decoding using Canonical Huffman code.
 */
class Huffman {
    std::map<int16_t, std::vector<bool>> code_book;     // code book for coding
    std::vector<uint16_t> code_lengths;
    std::vector<int16_t> code_letters;
    uint16_t max_code_len;

    static const int16_t HUFF_EOF;  // EOF in Huffman coding

    /**
     * @brief Calculates occurrence of symbols in a vector.
     * Returns map from symbols into the number of o occurrences.
     *
     * @param data Vector of symbols
     * @return std::map<int16_t, uint32_t> Map of symbols and their occurrences in a vector.
     */
    static std::map<int16_t, unsigned> get_alphabet_counts(std::vector<int16_t> data);

    /**
     * @brief Generates code book for Canonical Huffman code.
     *
     * @param data Data to be encoded.
     */
    void generate_code_book(std::vector<int16_t> data);

    public:
        /**
         * @brief Codes input data using Canonical Huffman code.
         *
         * @param data Input data to be codec.
         * @return bitdata::bits Returns a vector of coded bits.
         */
        bitdata::bits code(std::vector<int16_t> data);

        /**
         * @brief Decodes input data from iterator using Canonical Huffman code.
         * It changes the input iterator.
         *
         * @param reading_head Iterator pointing on bit data for decoding.
         * @return std::vector<int16_t> Vector of decoded data.
         */
        std::vector<int16_t> decode(bitdata::bits::iterator &reading_head);

};
