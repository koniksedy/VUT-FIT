#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>


class Decoder {
    bool model = false;
    bool adaptive = false;
    uint16_t width = 0;
    uint16_t height = 0;
    std::vector<bool> data_in;
    std::vector<uint8_t> data_out;

    public:
        bool load(char *input);
        void run();
        bool save(char *output);
};