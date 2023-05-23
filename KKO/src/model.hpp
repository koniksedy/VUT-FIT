/**
 * @file model.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file for linear model.
 * @date 04.05.2023
 */

#include <vector>
#include <set>
#include <cstdint>
#include "matrix.hpp"

#pragma once


/**
 * @brief A Model class for data preprocessing and data reconstruction.
 */
class Model {
    public:
        // Enumeration of possible model's passthru
        enum class PassThru {left_right, top_down, main_diagonal, side_diagonal};

        /**
         * @brief Model constructor. Construct model directly from input data
         * without any other computation.
         *
         * @param data Input data.
         */
        Model(matrix<uint8_t> data);

        /**
         * @brief Model constructor. Calculates model from input data and a specified pass.
         *
         * @param data Input data matrix.
         * @param pass Pass through a data matrix.
         */
        Model(matrix<uint8_t> data, PassThru pass);

        Model(matrix<int16_t> d, uint8_t mp, PassThru p) : data(d), master_point(mp), pass(p) {}

        /**
         * @brief Reconstruct data matrix from a model.
         *
         * @return matrix<uint8_t> Reconstructed matrix of an original data.
         */
        matrix<uint8_t> reconstruct();

        matrix<int16_t> get_data() {
            return this->data;
        }

        PassThru get_pass() {
            return this->pass;
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
        std::set<int16_t> uniq_num;     // a number of unique numbers (symbols)
        matrix<int16_t> data;           // a model data
        uint8_t master_point;           // a model master point
        PassThru pass;                  // a model passthru

        /**
         * @brief Sets top left point as a master point and calculates
         * the first row (from left to right) and the first column (from top to bottom).
         *
         * @param data Input data matrix.
         */
        void make_left_master_point(matrix<uint8_t> data);

        /**
         * @brief Sets top right point as a master point and calculates
         * the first row (from right to left) and the last column (from top to bottom).
         *
         * @param data Input data matrix.
         */
        void make_right_master_point(matrix<uint8_t> data);

        /**
         * @brief Calculates model from left to right for each row.
         *
         * @param data Input data matrix.
         */
        void make_left_right(matrix<uint8_t> data);

        /**
         * @brief Calculates model from top to bottom for each column.
         *
         * @param data Input data matrix.
         */
        void make_top_down(matrix<uint8_t> data);

        /**
         * @brief Calculates model on parallel with a main diagonal from top left
         * to the bottom right corner.
         *
         * @param data Input data matrix.
         */
        void make_main_diagonal(matrix<uint8_t> data);

        /**
         * @brief Calculates model on parallel with a side diagonal from top right
         * to the bottom left corner.
         *
         * @param data Input data matrix.
         */
        void make_side_diagonal(matrix<uint8_t> data);

        /**
         * @brief Reconstruct top left master point and reconstructs the first row
         * (from left to right) and the first column (from top to bottom).
         *
         * @param out A reference on an output matrix.
         */
        void reconstruct_right_master_point(matrix<uint8_t> &out);

        /**
         * @brief Reconstruct top right master point and reconstructs the first row
         * (from right to left) and the last column (from top to bottom).
         *
         * @param out A reference on an output matrix.
         */
        void reconstruct_left_master_point(matrix<uint8_t> &out);

        /**
         * @brief Reconstructs data from left to right for each row.
         *
         * @param out A reference on an output matrix.
         */
        void reconstruct_left_right(matrix<uint8_t> &out);

        /**
         * @brief Reconstructs data from top to bottom for each column.
         *
         * @param out A reference on an output matrix.
         */
        void reconstruct_top_down(matrix<uint8_t> &out);

        /**
         * @brief Reconstructs data on parallel with a main diagonal from top left
         * to the bottom right corner.
         *
         * @param out
         */
        void reconstruct_main_diagonal(matrix<uint8_t> &out);

        /**
         * @brief Reconstructs data on parallel with a side diagonal from top right
         * to the bottom left corner.
         *
         * @param out
         */
        void reconstruct_side_diagonal(matrix<uint8_t> &out);
};
