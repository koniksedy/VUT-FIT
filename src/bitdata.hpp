#include <vector>
#include <cstdint>

#pragma once

namespace bitdata
{
    using bits = std::vector<bool>;

    template<typename T>
    bits to_bits(T value);

    uint8_t bits_to_uint8(bitdata::bits::iterator data, std::size_t n);
    int16_t bits_to_int16(bitdata::bits::iterator data, std::size_t n);

};
