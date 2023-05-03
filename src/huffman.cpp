#include "huffman.hpp"
#include <iostream>


const int16_t Huffman::HUFF_EOF = -256;

std::map<int16_t, uint32_t> Huffman::get_alphabet_counts(std::vector<int16_t> data) {
    std::map<int16_t, uint32_t> alphabet_counts;
    for (const int16_t &a: data) {
        alphabet_counts[a]++;
    }
    return alphabet_counts;
}

void Huffman::generate_code_book(std::vector<int16_t> data) {

    std::map<int16_t, uint32_t> alpha_cnts = this->get_alphabet_counts(data);
    std::priority_queue<std::shared_ptr<Tree>, std::vector<std::shared_ptr<Tree>>, Tree_Val_Differencer> min_pqueue;
    std::vector<std::shared_ptr<Tree>> huffman_tree_leaves;
    huffman_tree_leaves.reserve(alpha_cnts.size()+1);

    for (auto const &v: alpha_cnts) {
        std::shared_ptr<Tree> tree(new Tree());
        tree->data = v.first;
        tree->cnt = v.second;
        tree->node_type = Tree::Node_Type::leaf;
        min_pqueue.push(tree);
        huffman_tree_leaves.push_back(tree);
    }

    std::shared_ptr<Tree> eof(new Tree());
    eof->data = this->HUFF_EOF;
    eof->cnt = 1;
    eof->node_type = Tree::Node_Type::leaf;
    min_pqueue.push(eof);
    huffman_tree_leaves.push_back(eof);

    std::shared_ptr<Tree> huffman_tree;
    while (min_pqueue.size() > 1) {
        std::shared_ptr<Tree> min_tree_a = min_pqueue.top();
        min_pqueue.pop();
        std::shared_ptr<Tree> min_tree_b = min_pqueue.top();
        min_pqueue.pop();

        std::shared_ptr<Tree> predecessor(new Tree());
        predecessor->cnt = min_tree_a->cnt + min_tree_b->cnt;
        predecessor->left = min_tree_a;
        predecessor->right = min_tree_b;
        predecessor->node_type = Tree::Node_Type::inner;

        min_pqueue.push(predecessor);
    }

    huffman_tree = min_pqueue.top();
    huffman_tree->calculate_depth();

    std::sort(huffman_tree_leaves.begin(), huffman_tree_leaves.end());

    for (std::size_t i = 0; i < huffman_tree_leaves.size(); ++i) {
        printf("%d -> cnt:%u depth:%u\n", huffman_tree_leaves[i]->data, huffman_tree_leaves[i]->cnt, huffman_tree_leaves[i]->depth);
    }

    int16_t leaf_data = huffman_tree_leaves[0]->data;
    uint16_t leaf_length = huffman_tree_leaves[0]->depth;
    this->max_code_len = 1;
    uint16_t prev_leaf_length = leaf_length;
    uint64_t leaf_code = 0;
    uint64_t prev_leaf_code = 0;
    this->code_book[leaf_data] = std::vector<bool>{0};

    for (std::size_t i = 1; i < huffman_tree_leaves.size(); ++i) {
        leaf_data = huffman_tree_leaves[i]->data;
        leaf_length = huffman_tree_leaves[i]->depth;
        leaf_code = (prev_leaf_code + 1) << (leaf_length - prev_leaf_length);

        this->code_book[leaf_data] = bitdata::to_bits(leaf_code, leaf_length);
        this->max_code_len = std::max(this->max_code_len, leaf_length);

        prev_leaf_code = leaf_code;
        prev_leaf_length = leaf_length;
    }

    this->code_lengths.resize(this->max_code_len, 0);
    for (auto const &v: this->code_book) {
        this->code_lengths[v.second.size()-1]++;
    }

    this->code_letters.resize(huffman_tree_leaves.size());
    for (std::size_t i = 0; i < huffman_tree_leaves.size(); ++i) {
        this->code_letters[i] = huffman_tree_leaves[i]->data;
    }

    // for (auto &v: this->code_book) {
    //     printf("%d = ", v.first);
    //     for (std::size_t b = 0; b < v.second.size(); ++b) {
    //         std::cout << (v.second[b] ? "1" : "0");
    //     }
    //     std::cout << std::endl;
    // }
}

/*
void Huffman::construct_tree() {
    // unused

    Tree *tree = new Tree();
    tree->node_type = Tree::Node_Type::inner;
    tree->depth = 0;
    tree->left = nullptr;
    tree->right = nullptr;

    int16_t letter;
    Tree *node;
    Tree *next_node;
    for (auto const &v: this->code_book) {
        letter = v.first;
        node = tree;
        for (std::size_t i = 0; i < v.second.size(); ++i) {
            next_node = v.second[i] == 0 ? node->right : node->left;
            if (next_node == nullptr) {
                next_node = new Tree();
                next_node->depth = node->depth + 1;
                next_node->node_type = Tree::Node_Type::inner;
                next_node->left = nullptr;
                next_node->right = nullptr;
                if (v.second[i] == 0) {
                    node->right = next_node;
                } else {
                    node->left = next_node;
                }
            }
            node = next_node;
        }
        node->node_type = Tree::Node_Type::leaf;
        node->data = letter;
    }
}
*/

bitdata::bits Huffman::code(std::vector<int16_t> data) {

    // bitdata::bits coded_data;
    // bitdata::bits data_raw;
    // for (std::size_t i = 0; i < data.size(); ++i) {
    //     data_raw = bitdata::to_bits(data[i], 16);
    //     coded_data.insert(coded_data.end(), data_raw.begin(), data_raw.end());
    // }
    // data_raw = bitdata::to_bits(static_cast<int16_t>(-32768), 16);
    // coded_data.insert(coded_data.end(), data_raw.begin(), data_raw.end());
    // return coded_data;

    bitdata::bits coded_data;

    this->generate_code_book(data);

    bitdata::bits max_code_len_b = bitdata::to_bits(this->max_code_len, 16);
    coded_data.insert(coded_data.end(), std::make_move_iterator(max_code_len_b.begin()), std::make_move_iterator(max_code_len_b.end()));

    for (std::size_t i = 0; i < this->code_lengths.size(); ++i) {
        bitdata::bits length_b = bitdata::to_bits(this->code_lengths[i], 9);
        coded_data.insert(coded_data.end(), std::make_move_iterator(length_b.begin()), std::make_move_iterator(length_b.end()));
    }

    for (std::size_t i = 0; i < this->code_letters.size(); ++i) {
        bitdata::bits letter_b = bitdata::to_bits(this->code_letters[i], 9);
        coded_data.insert(coded_data.end(), std::make_move_iterator(letter_b.begin()), std::make_move_iterator(letter_b.end()));
    }

    bitdata::bits code_bin;
    for (std::size_t i = 0; i < data.size(); ++i) {
        code_bin = this->code_book[data[i]];
        coded_data.insert(coded_data.end(), std::make_move_iterator(code_bin.begin()), std::make_move_iterator(code_bin.end()));
    }

    code_bin = this->code_book[this->HUFF_EOF];
    coded_data.insert(coded_data.end(), std::make_move_iterator(code_bin.begin()), std::make_move_iterator(code_bin.end()));

    return coded_data;
}

std::vector<int16_t> Huffman::decode(bitdata::bits::iterator &reading_head) {
    this->max_code_len = bitdata::bits_to_uint16(reading_head, 16);
    reading_head += 16;

    printf("Max code len = %u\n", this->max_code_len);

    std::cout << "CODE LENGTHS" << std::endl;
    this->code_lengths.resize(this->max_code_len);
    std::size_t letters_cnt = 0;
    for (std::size_t i = 0; i < this->max_code_len; ++i) {
        this->code_lengths[i] = bitdata::bits_to_uint16(reading_head, 9);
        reading_head += 9;
        letters_cnt += this->code_lengths[i];
        std::cout << this->code_lengths[i];
        std::cout << " ";
    }
    std::cout << std::endl;

    std::cout << "CODE LETTERS" << std::endl;
    this->code_letters.resize(letters_cnt);
    for (std::size_t i = 0; i < letters_cnt; ++i) {
        this->code_letters[i] = bitdata::bits_to_int16(reading_head, 9);
        reading_head += 9;
        std::cout << this->code_letters[i];
        std::cout << " ";
    }
    std::cout << std::endl;

    std::vector<uint64_t> first_code(this->max_code_len + 1);
    std::vector<size_t> first_symbol(this->max_code_len + 1);
    uint64_t c = 0;
    std::size_t s = 1;
    for (uint16_t i = 0; i < this->max_code_len; ++i) {
        first_code[i] = c;
        first_symbol[i] = s;
        s += this->code_lengths[i];
        c = (c + this->code_lengths[i]) << 1;
    }
    first_code[this->max_code_len] = c;
    first_symbol[this->max_code_len] = s;

    std::cout << "FIRST SYMBOL" << std::endl;
    for (std::size_t i = 0; i < first_symbol.size(); ++i) {
        std::cout << first_symbol[i];
        std::cout << " ";
    }
    std::cout << std::endl;

    std::cout << "FIRST CODE" << std::endl;
    for (std::size_t i = 0; i < first_code.size(); ++i) {
        std::cout << first_code[i];
        std::cout << " ";
    }
    std::cout << std::endl;


    std::vector<int16_t> out;

    c = 0;
    std::size_t l = 0;
    int16_t letter = 0;
    while (letter != this->HUFF_EOF) {

        c = (c << 1) + bitdata::bits_to_uint8(reading_head, 1);
        reading_head++;
        if ((c << 1) < first_code[l + 1]) {
            letter = this->code_letters[first_symbol[l] + c - first_code[l] - 1];
            if (letter != this->HUFF_EOF) {
                out.push_back(letter);
            }
            l = 0;
            c = 0;
        } else {
            l++;
        }
    }

    return out;
}
