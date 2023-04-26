#include "coder.hpp"
#include "bitdata.hpp"


bool Coder::load(char *input, uint16_t width) {
    std::ifstream stream(input, std::ios::in | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Input file not found." << std::endl;
        return false;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(stream)),
                              (std::istreambuf_iterator<char>()));

    this->width = width;
    this->height = data.size() / width;

    this->data_in.reserve(this->height);
    for (size_t i = 0; i < this->height; ++i) {
        std::vector<uint8_t> row(width);
        row.reserve(width);
        size_t last = std::min((i * width) + width, data.size());
        move(data.begin() + (i * width), data.begin() + last, row.begin());
        this->data_in.push_back(row);
    }

    return true;
}


void Coder::run() {

    /**********************************************
     * TEST - BITDATA                             *
     * ********************************************/
    /*
    uint8_t a = 107;
    bitdata::bits ab = bitdata::to_bits(a, 8);
    printf("uint8_t 107 = ");
    for (std::size_t i = 0; i < ab.size(); ++i) {
        std::cout << (ab[i] ? "1" : "0");
    }
    printf(" = %u\n", bitdata::bits_to_uint8(ab.begin(), 8));

    int8_t b = 75;
    bitdata::bits bb = bitdata::to_bits(b, 8);
    printf("int8_t 75 = ");
    for (std::size_t i = 0; i < bb.size(); ++i) {
        std::cout << (bb[i] ? "1" : "0");
    }
    printf(" = %d\n", bitdata::bits_to_int16(bb.begin(), 8));

    int8_t c = -75;
    bitdata::bits cb = bitdata::to_bits(c, 8);
    printf("int8_t -75 = ");
    for (std::size_t i = 0; i < cb.size(); ++i) {
        std::cout << (cb[i] ? "1" : "0");
    }
    printf(" = %d\n", bitdata::bits_to_int16(cb.begin(), 8));

    uint16_t d = 2541;
    bitdata::bits db = bitdata::to_bits(d, 16);
    printf("uint16_t 2541 = ");
    for (std::size_t i = 0; i < db.size(); ++i) {
        std::cout << (db[i] ? "1" : "0");
    }
    printf(" = %d\n", bitdata::bits_to_uint16(db.begin(), 16));

    int16_t e = 245;
    bitdata::bits eb = bitdata::to_bits(e, 16);
    printf("int16_t 245 = ");
    for (std::size_t i = 0; i < eb.size(); ++i) {
        std::cout << (eb[i] ? "1" : "0");
    }
    printf(" = %d\n", bitdata::bits_to_int16(eb.begin(), 16));

    int16_t f = -256;
    bitdata::bits fb = bitdata::to_bits(f, 9);
    printf("int9_t -256 = ");
    for (std::size_t i = 0; i < fb.size(); ++i) {
        std::cout << (fb[i] ? "1" : "0");
    }
    printf(" = %d\n", bitdata::bits_to_int16(fb.begin(), 9));
    */

    /**********************************************
     * TEST - LOAD                                *
     * ********************************************/
    /*
    std::cout << "Loaded" << std::endl;
    for (std::size_t i = 0; i < this->data_in.size(); ++i) {
        for (std::size_t j = 0; j < this->data_in[i].size(); ++j) {
            printf("%x(%u) ", this->data_in[i][j], this->data_in[i][j]);
        }
        std::cout << std::endl;
    }
    */
    /*********************************************/

    /**********************************************
     * TEST - Model                               *
     * ********************************************/
    /*
    std::cout << "Model left-right" << std::endl;
    Model model(this->data_in, Model::PassThru::left_right);
    matrix<int16_t> model_data = model.get_data();
    printf("%d\n", model.get_master_point());
    for (std::size_t i = 0; i < model_data.size(); ++i) {
        for (std::size_t j = 0; j < model_data[i].size(); ++j) {
            std::cout << model_data[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Reconstruct" << std::endl;
    Model model_reconstruct(model_data, model.get_master_point(), Model::PassThru::left_right);

    matrix<uint8_t> old_data = model_reconstruct.reconstruct();
    for (std::size_t i = 0; i < old_data.size(); ++i) {
        for (std::size_t j = 0; j < old_data[i].size(); ++j) {
            printf("%x(%u) ", old_data[i][j], old_data[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Model top-down" << std::endl;
    model = Model(this->data_in, Model::PassThru::top_down);
    model_data = model.get_data();
    printf("%d\n", model.get_master_point());
    for (std::size_t i = 0; i < model_data.size(); ++i) {
        for (std::size_t j = 0; j < model_data[i].size(); ++j) {
            std::cout << model_data[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Reconstruct" << std::endl;
    model_reconstruct = Model(model_data, model.get_master_point(), Model::PassThru::top_down);
    old_data = model_reconstruct.reconstruct();
    for (std::size_t i = 0; i < old_data.size(); ++i) {
        for (std::size_t j = 0; j < old_data[i].size(); ++j) {
            printf("%x(%u) ", old_data[i][j], old_data[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Model main-diagonal" << std::endl;
    model = Model(this->data_in, Model::PassThru::main_diagonal);
    model_data = model.get_data();
    printf("%d\n", model.get_master_point());
    for (std::size_t i = 0; i < model_data.size(); ++i) {
        for (std::size_t j = 0; j < model_data[i].size(); ++j) {
            std::cout << model_data[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Reconstruct" << std::endl;
    model_reconstruct = Model(model_data, model.get_master_point(), Model::PassThru::main_diagonal);
    old_data = model_reconstruct.reconstruct();
    for (std::size_t i = 0; i < old_data.size(); ++i) {
        for (std::size_t j = 0; j < old_data[i].size(); ++j) {
            printf("%x(%u) ", old_data[i][j], old_data[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Test - model
    std::cout << "Model side-diagonal" << std::endl;
    model = Model(this->data_in, Model::PassThru::side_diagonal);
    model_data = model.get_data();
    printf("%d\n", model.get_master_point());
    for (std::size_t i = 0; i < model_data.size(); ++i) {
        for (std::size_t j = 0; j < model_data[i].size(); ++j) {
            std::cout << model_data[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Reconstruct" << std::endl;
    model_reconstruct = Model(model_data, model.get_master_point(), Model::PassThru::side_diagonal);
    old_data = model_reconstruct.reconstruct();
    for (std::size_t i = 0; i < old_data.size(); ++i) {
        for (std::size_t j = 0; j < old_data[i].size(); ++j) {
            printf("%x(%u) ", old_data[i][j], old_data[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */
    /**********************************************/

    /**********************************************
     * TEST - SPLITTER                             *
    ***********************************************/
   /*
    uint8_t block_width = 3;
    std::cout << "Blocks" << std::endl;
    std::vector<matrix<uint8_t>> blocks = splitter::split(this->data_in, block_width);
    for (std::size_t k = 0; k < blocks.size(); ++k) {
            printf("Block %lu\n", k);
            for (std::size_t i = 0; i < blocks[k].size(); ++i) {
                for (std::size_t j = 0; j < blocks[k][i].size(); ++j) {
                    printf("%x(%d) ", blocks[k][i][j], blocks[k][i][j]);
                }
                std::cout << std::endl;
            }
    }

    std::cout << "Reconstructed" << std::endl;
    matrix<uint8_t> reconstruct = splitter::merge(blocks, this->width);
    for (std::size_t i = 0; i < reconstruct.size(); ++i) {
        for (std::size_t j = 0; j < reconstruct[i].size(); ++j) {
            printf("%x(%d) ", reconstruct[i][j], reconstruct[i][j]);
        }
        std::cout << std::endl;
    }
    */
    /**********************************************/


    /**********************************************
        * TEST - LINEARIZER
    ***********************************************/
    /*
    uint8_t block_width = 4;
    std::cout << "Blocks" << std::endl;
    std::vector<matrix<uint8_t>> blocks = splitter::split(this->data_in, block_width);

    std::vector<matrix<int16_t>> models;
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        models.push_back(Model(blocks[i], Model::PassThru::left_right).get_data());
    }

    std::vector<std::vector<int16_t>> models_linear = linearizer::linearize(models);

    for (std::size_t k = 0; k < models.size(); ++k) {
        printf("Block %lu\n", k);
        for (std::size_t i = 0; i < models[k].size(); ++i) {
            for (std::size_t j = 0; j < models[k][i].size(); ++j) {
                printf("(%d) ", models[k][i][j]);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "LINEAR: ";
        for (std::size_t i = 0; i < models_linear[k].size(); ++i) {
            std::cout << models_linear[k][i] << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Reconstruction" <<std::endl;
    std::vector<matrix<int16_t>> models_rec = linearizer::delinearize(models_linear, this->width);
    for (std::size_t k = 0; k < models_rec.size(); ++k) {
        printf("RBlock %lu\n", k);
        printf("LINEAR: ");
        for (std::size_t i = 0; i < models_linear[k].size(); ++i) {
            printf("(%d) ", models_linear[k][i]);
        }
        std::cout << std::endl;
        std::cout << std::endl;
        for (std::size_t i = 0; i < models_rec[k].size(); ++i) {
            for (std::size_t j = 0; j < models_rec[k][i].size(); ++j) {
                printf("(%d) ", models_rec[k][i][j]);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    */

}

bool Coder::save(char *output) {

    std::ofstream stream(output, std::ios::out | std::ios::binary);

    if (!stream.good()) {
        std::cout << "Output file was not created successfully." << std::endl;
        return false;
    }

    std::size_t i = 0;
    uint8_t ch;
    uint8_t d;
    while (i < this->data_out.size()) {
        ch = 0;
        for (std::size_t j = 0; j < 8; ++j) {
            if (i < this->data_out.size()) {
                d = this->data_out[i++];
                ch = ch | d;
            }
            ch = ch << 1;
        }
        stream.write(reinterpret_cast<char*>(&ch), 1);
    }
    return true;
}
