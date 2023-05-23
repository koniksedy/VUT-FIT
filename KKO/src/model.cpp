/**
 * @file model.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief An implementation linear model for coding/decoding.
 * @date 04.05.2023
 */

#include "model.hpp"


/**
 * @brief Model constructor. Construct model directly from input data
 * without any other computation.
 *
 * @param data Input data.
 */
Model::Model(matrix<uint8_t> data) {
    this->master_point = 0;
    this->pass = Model::PassThru::left_right;
    this->data.resize(data.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        this->data[i].resize(data[i].size());
        for (std::size_t j = 0; j < data[i].size(); ++j) {
            this->data[i][j] = data[i][j];
        }
    }
}

/**
 * @brief Model constructor. Calculates model from input data and a specified pass.
 *
 * @param data Input data matrix.
 * @param pass Pass through a data matrix.
 */
Model::Model(matrix<uint8_t> data, PassThru pass) {
    this->pass = pass;

    // Alloc a model data matrix.
    this->data.resize(data.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        this->data[i].resize(data[i].size());
    }

    // Select a pass.
    switch (pass)
    {
        case PassThru::left_right:
            this->make_left_master_point(data);
            this->make_left_right(data);
            break;

        case PassThru::top_down:
            this->make_left_master_point(data);
            this->make_top_down(data);
            break;

        case PassThru::main_diagonal:
            this->make_left_master_point(data);
            this->make_main_diagonal(data);
            break;

        case PassThru::side_diagonal:
            this->make_right_master_point(data);
            this->make_side_diagonal(data);
            break;
    }
}

/**
 * @brief Sets top left point as a master point and calculates
 * the first row (from left to right) and the first column (from top to bottom).
 *
 * @param data Input data matrix.
 */
void Model::make_left_master_point(matrix<uint8_t> data) {
    this->master_point = data[0][0];
    this->data[0][0] = 0;
    this->uniq_num.insert(0);
    int16_t val;
    for (std::size_t j = 1; j < data[0].size(); ++j) {
        val = data[0][j-1] - data[0][j];
        this->data[0][j] = val;
        this->uniq_num.insert(val);
    }
    for (std::size_t i = 1; i < data.size(); ++i) {
        val = data[i-1][0] - data[i][0];
        this->data[i][0] = val;
        this->uniq_num.insert(val);
    }
}

/**
 * @brief Sets top right point as a master point and calculates
 * the first row (from right to left) and the last column (from top to bottom).
 *
 * @param data Input data matrix.
 */
void Model::make_right_master_point(matrix<uint8_t> data) {
    std::size_t max_j = data[0].size() - 1;
    this->master_point = data[0][max_j];
    this->data[0][max_j] = 0;
    this->uniq_num.insert(0);
    int16_t val;
    for (std::size_t j = 1; j < data[0].size(); ++j) {
        val = data[0][max_j-j+1] - data[0][max_j-j];
        this->data[0][max_j-j] = val;
        this->uniq_num.insert(val);
    }
    for (std::size_t i = 1; i < data.size(); ++i) {
        val = data[i-1][max_j] - data[i][max_j];
        this->data[i][max_j] = val;
        this->uniq_num.insert(val);
    }
}

/**
 * @brief Calculates model from left to right for each row.
 *
 * @param data Input data matrix.
 */
void Model::make_left_right(matrix<uint8_t> data)
{
    int16_t val;
    for (std::size_t i = 0; i < data.size(); ++i) {
        for (std::size_t j = 1; j < data[i].size(); ++j) {
            val = data[i][j-1] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}

/**
 * @brief Calculates model from top to bottom for each column.
 *
 * @param data Input data matrix.
 */
void Model::make_top_down(matrix<uint8_t> data)
{
    int16_t val;
    for (std::size_t j = 0; j < data[0].size(); ++j) {
        for (std::size_t i = 1; i < data.size(); ++i) {
            val = data[i-1][j] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}

/**
 * @brief Calculates model on parallel with a main diagonal from top left
 * to the bottom right corner.
 *
 * @param data Input data matrix.
 */
void Model::make_main_diagonal(matrix<uint8_t> data)
{
    int16_t val;
    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 1; j < data[0].size(); ++j) {
            val = data[i-1][j-1] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}

/**
 * @brief Calculates model on parallel with a side diagonal from top right
 * to the bottom left corner.
 *
 * @param data Input data matrix.
 */
void Model::make_side_diagonal(matrix<uint8_t> data)
{
    int16_t val;
    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 0; j < data[0].size()-1; ++j) {
            val = data[i-1][j+1] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}

/**
 * @brief Reconstruct data matrix from a model.
 *
 * @return matrix<uint8_t> Reconstructed matrix of an original data.
 */
matrix<uint8_t> Model::reconstruct() {
    // Alloc output matrix.
    matrix<uint8_t> out(this->data.size());
    for (std::size_t i = 0; i < this->data.size(); ++i) {
        out[i].resize(this->data[i].size());
    }

    // Select reconstruction method based on the pass.
    switch (this->pass) {
        case PassThru::left_right:
            this->reconstruct_left_master_point(out);
            this->reconstruct_left_right(out);
            break;

        case PassThru::top_down:
            this->reconstruct_left_master_point(out);
            this->reconstruct_top_down(out);
            break;

        case PassThru::main_diagonal:
            this->reconstruct_left_master_point(out);
            this->reconstruct_main_diagonal(out);
            break;

        case PassThru::side_diagonal:
            this->reconstruct_right_master_point(out);
            this->reconstruct_side_diagonal(out);
            break;
    }

    return out;
}

/**
 * @brief Reconstruct top left master point and reconstructs the first row
 * (from left to right) and the first column (from top to bottom).
 *
 * @param out A reference on an output matrix.
 */
void Model::reconstruct_left_master_point(matrix<uint8_t> &out) {
    for (std::size_t i = 0; i < this->data.size(); ++i) {
        if (i == 0) {
            out[0][0] = this->master_point;
        } else {
            out[i][0] = out[i-1][0] - this->data[i][0];
        }
    }

    for (std::size_t j = 1; j < this->data[0].size(); ++j) {
        out[0][j] = out[0][j-1] - this->data[0][j];
    }
}

/**
 * @brief Reconstruct top right master point and reconstructs the first row
 * (from right to left) and the last column (from top to bottom).
 *
 * @param out A reference on an output matrix.
 */
void Model::reconstruct_right_master_point(matrix<uint8_t> &out) {
    std::size_t max_j = this->data[0].size() - 1;
    for (std::size_t i = 0; i < this->data.size(); ++i) {
        if (i == 0) {
            out[0][max_j] = this->master_point;
        } else {
            out[i][max_j] = out[i-1][max_j] - this->data[i][max_j];
        }
    }
    for (std::size_t j = 1; j < this->data[0].size(); ++j) {
        out[0][max_j-j] = out[0][max_j-j+1] - this->data[0][max_j-j];
    }
}

/**
 * @brief Reconstructs data from left to right for each row.
 *
 * @param out A reference on an output matrix.
 */
void Model::reconstruct_left_right(matrix<uint8_t> &out) {
    for (std::size_t i = 0; i < this->data.size(); ++i) {
        for (std::size_t j = 1; j < this->data[i].size(); ++j) {
            out[i][j] = out[i][j-1] - this->data[i][j];
        }
    }
}

/**
 * @brief Reconstructs data from top to bottom for each column.
 *
 * @param out A reference on an output matrix.
 */
void Model::reconstruct_top_down(matrix<uint8_t> &out) {
    for (std::size_t j = 0; j < this->data[0].size(); ++j) {
        for (std::size_t i = 1; i < this->data.size(); ++i) {
            out[i][j] = out[i-1][j] - this->data[i][j];
        }
    }
}

/**
 * @brief Reconstructs data on parallel with a main diagonal from top left
 * to the bottom right corner.
 *
 * @param out
 */
void Model::reconstruct_main_diagonal(matrix<uint8_t> &out) {
    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 1; j < data[0].size(); ++j) {
            out[i][j] = out[i-1][j-1] - this->data[i][j];
        }
    }
}

/**
 * @brief Reconstructs data on parallel with a side diagonal from top right
 * to the bottom left corner.
 *
 * @param out
 */
void Model::reconstruct_side_diagonal(matrix<uint8_t> &out) {
    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 0; j < data[0].size()-1; ++j) {
            out[i][j] = out[i-1][j+1] - this->data[i][j];
        }
    }
}
