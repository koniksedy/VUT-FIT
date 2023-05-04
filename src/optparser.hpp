/**
 * @file optparser.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file for program parameters parser.
 * @date 04.05.2023
 */

#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <limits.h>

#pragma once


/**
 * @brief A class for parsing program parameters.
 */
struct OptParser {
    class Help {};  // An exception class for help.
    enum class Mode {nothing, code, decode};    // Program modes

    char *input = nullptr;          // input file name
    char *output = nullptr;         // output file name
    Mode mode = Mode::nothing;      // program mode
    bool adaptive = false;          // is required adaptive coding
    bool with_model = false;        // is required usage of a model
    uint16_t width = 0;             // width of an input image

    /**
     * @brief Prints help.
     */
    static void print_help();

    /**
     * @brief Parses program parameters.
     *
     * @param argc A number of program parameters.
     * @param argv A list of program parameters.
     */
    void parse(int argc, char **argv);
};
