/**
 * @file optparser.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief An implementation program parameters parser.
 * @date 04.05.2023
 */

#include "optparser.hpp"


/**
 * @brief Prints help.
 */
void OptParser::print_help()
{
    std::cerr << "usage: huff_codec [-h] (-c ([-m] [-a] -w width)| -d) -i input -o output  " << std::endl;
    std::cerr << "" << std::endl;
    std::cerr << "options:" << std::endl;
    std::cerr << "-h, --help  show this help message and exit" << std::endl;
    std::cerr << "-i          Input file." << std::endl;
    std::cerr << "-o          Output file." << std::endl;
    std::cerr << "-c          Huffman Coding" << std::endl;
    std::cerr << "-d          Huffman Decoding" << std::endl;
    std::cerr << "-m          Linear model will be used." << std::endl;
    std::cerr << "-a          Adaptive scanning will be used." << std::endl;
    std::cerr << "-w          Input image width." << std::endl;
}

/**
 * @brief Parses program parameters.
 *
 * @param argc A number of program parameters.
 * @param argv A list of program parameters.
 */
void OptParser::parse(int argc, char **argv)
{
    int opt;
    int w;
    while ((opt = getopt(argc, argv, "cdmai:o:w:h")) != -1)
    {
        switch (opt)
        {
            case 'c':
                if (this->mode == Mode::decode) {
                    throw std::invalid_argument("Invalid argument: -c and -d can not be set at the same time.");
                }
                this->mode = Mode::code;
                break;

            case 'd':
                if (this->mode == Mode::code) {
                    throw std::invalid_argument("Invalid argument: -c and -d can not be set at the same time.");
                }
                this->mode = Mode::decode;
                break;

            case 'm':
                this->with_model = true;
                break;

            case 'a':
                this->adaptive = true;
                break;

            case 'i':
                this->input = optarg;
                break;

            case 'o':
                this->output = optarg;
                break;

            case 'w':
                try {
                    w = std::stoi(optarg, nullptr, 10);
                }
                catch(std::invalid_argument &ex) {
                    throw std::invalid_argument("Invalid argument value: -w must be withing the interval [1; 65535].");
                }
                catch(std::out_of_range &ex) {
                    throw std::invalid_argument("Invalid argument value: -w must be withing the interval [1; 65535].");
                }

                if (w < 1 || w > UINT16_MAX)
                {
                    throw std::invalid_argument("Invalid argument value: -w must be withing the interval [1; 65535].");
                }
                this->width = static_cast<uint16_t>(w);
                break;

            case 'h':
                this->print_help();
                throw Help();
                break;

            default:
                throw std::invalid_argument("Unknown parameter.");
                break;
        }
    }

    if (this->mode == Mode::nothing) {
        throw std::invalid_argument("-c or -d must be specified.");
    }
    if (this->mode == Mode::code && this->width == 0) {
        throw std::invalid_argument("The width must be specified.");
    }
    if (this->input == nullptr) {
        throw std::invalid_argument("The input must be specified.");
    }
    if (this->output == nullptr) {
        throw std::invalid_argument("The output must be specified.");
    }
}
