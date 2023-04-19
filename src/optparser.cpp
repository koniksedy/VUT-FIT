#include "optparser.hpp"


void OptParser::print_help()
{
    std::cerr << "Help. TODO." << std::endl;
}

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
