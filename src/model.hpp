#include <vector>
#include <set>
#include <cstdint>


class Model {
    using matrix_int16_t = std::vector<std::vector<int16_t>>;

    public:
        enum class PassThru {left_right, top_down, main_diagonal, side_diagonal};

        Model(matrix_int16_t data, PassThru pass);

        matrix_int16_t get_data() {
            return this->data;
        }

        uint8_t get_master_point() {
            return this->master_point;
        }

        int get_unique_num_cnt() {
            return this->uniq_num.size();
        }

    private:
        std::set<int> uniq_num;
        matrix_int16_t data;
        uint8_t master_point;
        PassThru pass;

        void make_left_master_point(matrix_int16_t data);
        void make_right_master_point(matrix_int16_t data);
        void make_left_right(matrix_int16_t data);
        void make_top_down(matrix_int16_t data);
        void make_main_diagonal(matrix_int16_t data);
        void make_side_diagonal(matrix_int16_t data);

};
