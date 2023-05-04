/**
 * @file coder.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file of a general coder.
 * @date 04.05.2023
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "matrix.hpp"
#include "splitter.hpp"
#include "linearizer.hpp"
#include "model.hpp"
#include "huffman.hpp"

#pragma once


/**
 * @brief A Coder class for loading, coding and saving.
 */
class Coder {
    bool model = false;             // is model used
    bool adaptive = false;          // is adaptive coding used
    uint16_t width = 0;             // width of an input picture
    matrix<uint8_t> data_in;        // raw input
    std::vector<bool> data_out;     // output

    /**
     * @brief Appends data from bit vector at the end of output data using move operation.
     *
     * @param bit_data_raw Bit vector to be moved.
     */
    inline void move_back_to_output(bitdata::bits bit_data_raw);

    public:
        void set_model(bool is_model) {
            this->model = is_model;
        }

        void set_adaptive(bool is_adaptive) {
            this->adaptive = is_adaptive;
        }

        /**
         * @brief Loads raw data from file.
         *
         * @param input File name.
         * @param width Width of coded image.
         * @return true: Successful load from file.
         * @return false: Unsuccessful load from file.
         */
        bool load(char *input, uint16_t width);

        /**
         * @brief Codes loaded data.
         */
        void run();

        /**
         * @brief Saves coded data into the output file.
         *
         * @param output File name.
         * @return true: Successful save.
         * @return false: Unsuccessful save.
         */
        bool save(char *output);
};
