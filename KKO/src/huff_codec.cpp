/**
 * @file huff_codec.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A main file of a program for coding/decoding using canonical Huffman
 * code with an adaptive scanning.
 * @date 04.05.2023
 */

#include <iostream>
#include "optparser.hpp"
#include "coder.hpp"
#include "decoder.hpp"


int main(int argc, char **argv) {
    // Parse program arguments
    OptParser parser;
    try {
        parser.parse(argc, argv);
    }
    catch (const std::invalid_argument &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    catch (OptParser::Help &ex) {
        return 0;
    }


    if (parser.mode == OptParser::Mode::code) {
        // Coding
        Coder coder;
        coder.set_adaptive(parser.adaptive);
        coder.set_model(parser.with_model);
        if (!coder.load(parser.input, parser.width)) {
            std::cerr << "Reading of the input file " << parser.input << " was not successful." << std::endl;
            return 1;
        }
        coder.run();
        coder.save(parser.output);

    } else {
        // Decoding
        Decoder decoder;
        if (!decoder.load(parser.input)) {
            std::cerr << "Reading of the input file " << parser.input << " was not successful." << std::endl;
            return 1;
        }
        decoder.run();
        decoder.save(parser.output);
    }

    return 0;
}
