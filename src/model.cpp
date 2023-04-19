#include "model.hpp"


Model::Model(matrix_int16_t data, PassThru pass) {
    this->pass = pass;

    this->data.reserve(data.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        std::vector<int16_t> row(data[i].size());
        this->data[i] = row;
    }

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
            this->make_right_master_point(data);
            this->make_top_down(data);
            break;

        case PassThru::side_diagonal:
            this->make_right_master_point(data);
            this->make_side_diagonal(data);
            break;
    }
}

void Model::make_left_master_point(matrix_int16_t data) {
    this->master_point = data[0][0];
    this->data[0][0] = 0;
    this->uniq_num.insert(0);
    int16_t val;
    for (std::size_t j = 1; j < data[0].size(); ++j) {
        val = this->data[0][j-1] - data[0][j];
        this->data[0][j] = val;
        this->uniq_num.insert(val);
    }
    for (std::size_t i = 1; i < data.size(); ++i) {
        val = this->data[i-1][0] - data[i][0];
        this->data[i][0] = val;
        this->uniq_num.insert(val);
    }
}

void Model::make_right_master_point(matrix_int16_t data) {
    std::size_t max_j = data[0].size() - 1;
    this->master_point = data[0][max_j];
    this->data[0][max_j] = 0;
    this->uniq_num.insert(0);
    int16_t val;
    for (std::size_t j = max_j - 1; j >= 0; --j) {
        val = this->data[0][j+1] - data[0][j];
        this->data[0][j] = val;
        this->uniq_num.insert(val);
    }
    for (std::size_t i = 1; i < data.size(); ++i) {
        val = this->data[i-1][max_j] - data[i][max_j];
        this->data[i][max_j] = val;
        this->uniq_num.insert(val);
    }
}

void Model::make_left_right(matrix_int16_t data)
{
    int16_t val;
    for (std::size_t i = 0; i < data.size(); ++i) {
        for (std::size_t j = 1; j < data[i].size(); ++j) {
            val = this->data[i][j-1] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}

void Model::make_top_down(Model::matrix_int16_t data)
{
    int16_t val;
    for (std::size_t j = 0; j < data[0].size(); ++j) {
        for (std::size_t i = 1; i < data.size(); ++i) {
            val = this->data[i-1][j] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}

void Model::make_main_diagonal(Model::matrix_int16_t data)
{
    int16_t val;
    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 1; j < data[0].size(); ++j) {
            val = this->data[i-1][j-1] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}

void Model::make_side_diagonal(Model::matrix_int16_t data)
{
    int16_t val;
    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 0; j < data[0].size()-1; ++j) {
            val = this->data[i-1][j+1] - data[i][j];
            this->data[i][j] = val;
            this->uniq_num.insert(val);
        }
    }
}
