/**
 * @file bitdata.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file for bit representation and operations.
 * @date 04.05.2023
 */

#include <vector>
#include <cstdint>
#include <iostream>

#pragma once

/**
 * @brief Namespace containing bit representation and operations.
 */
namespace bitdata
{
    using bits = std::vector<bool>;

    /**
     * @brief Template implementing conversion of value of a type T into
     * a bit vector using n LSb.
     *
     * @tparam T
     * @param value Value to be converted.
     * @param size Number of value's LSb to be used.
     * @return bits: A vector of bits (booleans).
     */
    template<typename T>
    bits to_bits(T value, std::size_t size) {
        bitdata::bits out(size);
        for (std::size_t i = 0; i < size; ++i) {
            out[size - i - 1] = static_cast<bool>((value >> i) & 1);
        }
        return out;
    }

    /**
     * @brief Converts n bits from iterator into one uint8_t.
     * The input iterator won't be changed.
     *
     * @param data Input iterator.
     * @param n Number of bits to be taken.
     * @return uint8_t: Converted number.
     */
    uint8_t bits_to_uint8(bitdata::bits::iterator data, std::size_t n);

    /**
     * @brief Converts n bits from iterator into one uint16_t.
     * The input iterator won't be changed.
     *
     * @param data Input iterator.
     * @param n Number of bits to be taken.
     * @return uint16_t: Converted number.
     */
    uint16_t bits_to_uint16(bitdata::bits::iterator data, std::size_t n);

    /**
     * @brief Converts n bits from iterator into one int16_t.
     * The input iterator won't be changed.
     *
     * @param data Input iterator.
     * @param n Number of bits to be taken.
     * @return int16_t: Converted number.
     */
    int16_t bits_to_int16(bitdata::bits::iterator data, std::size_t n);

};
