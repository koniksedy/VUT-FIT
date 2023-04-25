#include "bitdata.hpp"


template<typename T>
bitdata::bits bitdata::to_bits(T value) {
    std::size_t data_len = sizeof(value) * 8;
    bitdata::bits out(data_len);

    for (std::size_t i = 0; i < data_len; ++i) {
        out[data_len - i - 1] = static_cast<bool>((value >> i) & 1);
    }

    return out;
}

uint8_t bitdata::bits_to_uint8(bitdata::bits::iterator data, std::size_t n) {
    uint8_t out = 0;
    for (int i = 0; i < n; ++i) {
        out = out << 1;
        out = out | static_cast<uint8_t>(*data);
        data++;
    }
    return out;
}

int16_t bitdata::bits_to_int16(bitdata::bits::iterator data, std::size_t n) {
    int16_t out = 0;
    for (int i = 0; i < n; ++i) {
        out = out << 1;
        out = out | static_cast<int16_t>(*data);
        data++;
    }

    if (n != 16 && (out >> (n - 1)) == 1) {
        out = (0xff << n) | out;
    }

    return out;
}
