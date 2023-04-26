#include <vector>
#include <cstdint>

#pragma once


namespace bitdata
{
    using bits = std::vector<bool>;

    template<typename T>
    bits to_bits(T value, std::size_t size) {
        bitdata::bits out(size);

        for (std::size_t i = 0; i < size; ++i) {
            out[size - i - 1] = static_cast<bool>((value >> i) & 1);
        }

        return out;
    }

    uint8_t bits_to_uint8(bitdata::bits::iterator data, std::size_t n);
    uint16_t bits_to_uint16(bitdata::bits::iterator data, std::size_t n);
    int16_t bits_to_int16(bitdata::bits::iterator data, std::size_t n);

};
