#include <vector>
#include <cstdint>
#include <stdexcept>
#include "matrix.hpp"


struct Splitter {

        template<typename T>
        std::vector<matrix<T>> split(matrix<T> data, uint8_t block_width);

        template<typename T>
        matrix<T> merge(std::vector<matrix<T>> data, uint16_t output_width);
};
