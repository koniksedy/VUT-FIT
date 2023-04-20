#include <vector>
#include <cstdint>
#include <stdexcept>


struct Splitter {
    using matrix_int16_t = std::vector<std::vector<int16_t>>;

    public:
        std::vector<matrix_int16_t> split(matrix_int16_t data, uint8_t block_width);

        matrix_int16_t merge(std::vector<matrix_int16_t> data, uint16_t output_width);
};
