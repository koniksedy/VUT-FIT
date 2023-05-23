/**
 * @file decoder.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file of a general decoder.
 * @date 04.05.2023
 */

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


/**
 * @brief A Coder class for loading, decoding and saving.
 */
class Decoder {
    bool model = false;         // was model used
    bool adaptive = false;      // was adaptive coding used
    uint16_t width = 0;         // width of an image
    bitdata::bits data_in;      // coded data
    matrix<uint8_t> data_out;   // raw output data

    /**
     * @brief Tests if the iterator near (within maximal distance) the end.
     *
     * @param iter Iterator.
     * @param distance Maximum distance from the end.
     * @return true: The end is closer than distance.
     * @return false: The end is further than distance.
     */
    bool is_near_end(bitdata::bits::iterator iter, std::size_t distance);

    public:
        /**
         * @brief Loads data from file.
         *
         * @param input File name.
         * @return true: Successful load.
         * @return false: Unsuccessful load.
         */
        bool load(char *input);

        /**
         * @brief Decodes of loaded data.
         */
        void run();

        /**
         * @brief Saves decoded data to the file.
         *
         * @param output File name.
         * @return true: Successful save.
         * @return false: Unsuccessful save.
         */
        bool save(char *output);
};
