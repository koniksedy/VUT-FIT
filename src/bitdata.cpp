#include "bitdata.hpp"


uint8_t bitdata::bits_to_uint8(bitdata::bits::iterator data, std::size_t n) {
    uint8_t out = 0;
    for (std::size_t i = 0; i < n; ++i) {
        out = out << 1;
        out = out | static_cast<uint8_t>(*data);
        data++;
    }
    return out;
}

uint16_t bitdata::bits_to_uint16(bitdata::bits::iterator data, std::size_t n) {
    uint16_t out = 0;
    for (std::size_t i = 0; i < n; ++i) {
        out = out << 1;
        out = out | static_cast<uint16_t>(*data);
        data++;
    }
    return out;
}

int16_t bitdata::bits_to_int16(bitdata::bits::iterator data, std::size_t n) {
    int16_t out = 0;
    for (std::size_t i = 0; i < n; ++i) {
        out = out << 1;
        out = out | static_cast<int16_t>(*data);
        data++;
    }

    if (n != 16 && (out >> (n - 1)) == 1) {
        out = (0xff << n) | out;
    }

    return out;
}
