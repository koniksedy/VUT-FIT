#include "model.hpp"


Model::Model(matrix<uint8_t> data, PassThru pass) {
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
            this->make_left_master_point(data);
            this->make_main_diagonal(data);
            break;

        case PassThru::side_diagonal:
            this->make_right_master_point(data);
            this->make_side_diagonal(data);
            break;
    }
}

void Model::make_left_master_point(matrix<uint8_t> data) {
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

void Model::make_right_master_point(matrix<uint8_t> data) {
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

void Model::make_left_right(matrix<uint8_t> data)
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

void Model::make_top_down(matrix<uint8_t> data)
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

void Model::make_main_diagonal(matrix<uint8_t> data)
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

void Model::make_side_diagonal(matrix<uint8_t> data)
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

matrix<uint8_t> Model::reconstruct() {
    switch (this->pass) {
        case PassThru::left_right:
            return reconstruct_left_right();

        case PassThru::top_down:
            return reconstruct_top_down();

        case PassThru::main_diagonal:
            return reconstruct_main_diagonal();

        case PassThru::side_diagonal:
            return reconstruct_side_diagonal();
    }
}

matrix<uint8_t> Model::reconstruct_left_right() {

    matrix<uint8_t> out(this->data.size());

    for (std::size_t i = 0; i < this->data.size(); ++i) {
        out[i].reserve(this->data[i].size());
        if (i == 0) {
            out[0][0] = this->master_point;
        } else {
            out[i][0] = out[i-1][0] - this->data[i][0];
        }
    }

    for (std::size_t i = 0; i < this->data.size(); ++i) {
        for (std::size_t j = 1; j < this->data[i].size(); ++j) {
            out[i][j] = out[i][j-1] - this->data[i][j];
        }
    }

    return out;
}

matrix<uint8_t> Model::reconstruct_top_down() {

    matrix<uint8_t> out(this->data.size());

    for (std::size_t i = 0; i < this->data.size(); ++i) {
        out[i].reserve(this->data[i].size());
    }

    out[0][0] = this->master_point;
    for (std::size_t j = 1; j < this->data[0].size(); ++j) {
        out[0][j] = out[0][j-1] - this->data[0][j];
    }


    for (std::size_t j = 0; j < this->data[0].size(); ++j) {
        for (std::size_t i = 1; i < this->data.size(); ++i) {
            out[i][j] = out[i-1][j] - this->data[i][j];
        }
    }

    return out;
}

matrix<uint8_t> Model::reconstruct_main_diagonal() {

    matrix<uint8_t> out(this->data.size());

    for (std::size_t i = 0; i < this->data.size(); ++i) {
        out[i].reserve(this->data[i].size());
        if (i == 0) {
            out[0][0] = this->master_point;
        } else {
            out[i][0] = out[i-1][0] - this->data[i][0];
        }
    }
    for (std::size_t j = 1; j < this->data[0].size(); ++j) {
        out[0][j] = out[0][j-1] - this->data[0][j];
    }

    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 1; j < data[0].size(); ++j) {
            out[i][j] = out[i-1][j-1] - this->data[i][j];
        }
    }

    return out;
}

matrix<uint8_t> Model::reconstruct_side_diagonal() {

    matrix<uint8_t> out(this->data.size());

    std::size_t last_idx = this->data[0].size() - 1;
    for (std::size_t i = 0; i < this->data.size(); ++i) {
        out[i].reserve(this->data[i].size());
        if (i == 0) {
            out[0][last_idx] = this->master_point;
        } else {
            out[i][last_idx] = out[i-1][last_idx] - this->data[i][last_idx];
        }
    }
    for (std::size_t j = last_idx; j > 0; --j) {
        out[0][j] = out[0][j+1] - this->data[0][j];
    }

    for (std::size_t i = 1; i < data.size(); ++i) {
        for (std::size_t j = 0; j < data[0].size()-1; ++j) {
            out[i][j] = out[i-1][j+1] - this->data[i][j];
        }
    }

    return out;
}
