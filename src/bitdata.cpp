/**
 * @file bitdata.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief An implementation of bit operations.
 * @date 04.05.2023
 */

#include "bitdata.hpp"


/**
 * @brief Converts n bits from iterator into one uint8_t.
 * The input iterator won't be changed.
 *
 * @param data Input iterator.
 * @param n Number of bits to be taken.
 * @return uint8_t: Converted number.
 */
uint8_t bitdata::bits_to_uint8(bitdata::bits::iterator data, std::size_t n) {
    uint8_t out = 0;
    for (std::size_t i = 0; i < n; ++i) {
        out = out << 1;
        out = out | static_cast<uint8_t>(*data);
        data++;
    }
    return out;
}

/**
 * @brief Converts n bits from iterator into one uint16_t.
 * The input iterator won't be changed.
 *
 * @param data Input iterator.
 * @param n Number of bits to be taken.
 * @return uint16_t: Converted number.
 */
uint16_t bitdata::bits_to_uint16(bitdata::bits::iterator data, std::size_t n) {
    uint16_t out = 0;
    for (std::size_t i = 0; i < n; ++i) {
        out = out << 1;
        out = out | static_cast<uint16_t>(*data);
        data++;
    }
    return out;
}

/**
 * @brief Converts n bits from iterator into one int16_t.
 * The input iterator won't be changed.
 *
 * @param data Input iterator.
 * @param n Number of bits to be taken.
 * @return int16_t: Converted number.
 */
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
