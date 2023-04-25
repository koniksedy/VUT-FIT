#include <vector>
#include <set>
#include <cstdint>
#include "matrix.hpp"

#pragma once


class Model {
    public:
        enum class PassThru {left_right, top_down, main_diagonal, side_diagonal};

        Model(matrix<uint8_t> data, PassThru pass);

        Model(matrix<int16_t> d, uint8_t mp, PassThru p) : data(d), master_point(mp), pass(p) {}

        matrix<uint8_t> reconstruct();

        matrix<int16_t> get_data() {
            return this->data;
        }

        uint8_t get_master_point() {
            return this->master_point;
        }

        bool operator<(const Model &m) const {
            return this->uniq_num.size() < m.uniq_num.size();
        }

        bool operator>(const Model &m) const {
            return this->uniq_num.size() > m.uniq_num.size();
        }

    private:
        std::set<int16_t> uniq_num;
        matrix<int16_t> data;
        uint8_t master_point;
        PassThru pass;

        void make_left_master_point(matrix<uint8_t> data);
        void make_right_master_point(matrix<uint8_t> data);
        void make_left_right(matrix<uint8_t> data);
        void make_top_down(matrix<uint8_t> data);
        void make_main_diagonal(matrix<uint8_t> data);
        void make_side_diagonal(matrix<uint8_t> data);

        matrix<uint8_t> reconstruct_left_right();
        matrix<uint8_t> reconstruct_top_down();
        matrix<uint8_t> reconstruct_main_diagonal();
        matrix<uint8_t> reconstruct_side_diagonal();



};
