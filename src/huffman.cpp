#include "huffman.hpp"


std::map<int16_t, unsigned> Huffman::get_alphabet_counts(std::vector<int16_t> data) {
    std::map<int16_t, unsigned> alphabet_counts;
    for (const int16_t &a: data) {
        alphabet_counts[a]++;
    }
    return alphabet_counts;

}

void Huffman::generate_code_book(std::vector<int16_t> data) {

    std::map<int16_t, unsigned> alpha_cnts = this->get_alphabet_counts(data);
    std::priority_queue<Tree*, std::vector<Tree*>, Tree_Val_Differencer> min_pqueue;
    std::vector<Tree*> huffman_tree_leaves;

    std::size_t i = 0;
    for (auto const &v: alpha_cnts) {
        Tree *tree = new Tree();
        tree->data = v.first;
        tree->cnt = v.second;
        tree->node_type = Tree::Node_Type::leave;
        tree->left = nullptr;
        tree->right = nullptr;
        min_pqueue.push(tree);
        huffman_tree_leaves.push_back(tree);
    }

    Tree *eof = new Tree();
    eof->data = 256;
    eof->cnt = 1;
    eof->node_type = Tree::Node_Type::leave;
    eof->left = nullptr;
    eof->right = nullptr;
    min_pqueue.push(eof);
    huffman_tree_leaves.push_back(eof);

    Tree *huffman_tree;
    while (min_pqueue.size() > 1) {
        Tree *min_tree_a = min_pqueue.top();
        Tree *min_tree_b = min_pqueue.top();

        Tree *predecessor = new Tree();
        predecessor->cnt = min_tree_a->cnt + min_tree_b->cnt;
        predecessor->left = min_tree_a;
        predecessor->right = min_tree_b;
        predecessor->node_type = Tree::Node_Type::inner;

        min_pqueue.push(predecessor);
    }

    huffman_tree = min_pqueue.top();
    huffman_tree->calculate_codes();

    std::sort(huffman_tree_leaves.begin(), huffman_tree_leaves.end());

    int16_t leave_data = huffman_tree_leaves[0]->data;
    uint16_t leave_length = 1;
    this->max_code_len = 1;
    uint16_t prev_leave_length = 1;
    uint64_t leave_code = 0;
    uint64_t prev_leave_code = 0;
    this->code_book[leave_data] = std::vector<bool>{0};
    for (std::size_t i = 1; i < huffman_tree_leaves.size(); ++i) {
        leave_data = huffman_tree_leaves[i]->data;
        leave_length = huffman_tree_leaves[i]->depth + 1;
        leave_code = (prev_leave_code + 1) << (leave_length - prev_leave_length);

        this->code_book[leave_data] = bitdata::to_bits(leave_code);
        this->max_code_len = std::max(this->max_code_len, leave_length);

        prev_leave_code = leave_code;
        prev_leave_length = leave_length;
    }

    this->code_lengths.reserve(this->max_code_len-1);
    for (auto const &v: this->code_book) {
        this->code_lengths[v.second.size()-1]++;
    }

    this->code_letters.reserve(huffman_tree_leaves.size());
    for (std::size_t i = 0; i < huffman_tree_leaves.size(); ++i) {
        this->code_letters[i] = huffman_tree_leaves[i]->data;
    }
}

void Huffman::construct_tree() {

    Tree *tree;
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
        node->node_type = Tree::Node_Type::leave;
        node->data = letter;
    }
}

std::vector<bool> Huffman::code(std::vector<int16_t> data) {

    std::vector<bool> coded_data;

    this->generate_code_book(data);

    bitdata::bits max_code_len_b = bitdata::to_bits(this->max_code_len - 1);
    coded_data.insert(coded_data.end(), std::make_move_iterator(max_code_len_b.begin()), std::make_move_iterator(max_code_len_b.end()));

    for (std::size_t i = 0; i < this->code_lengths.size(); ++i) {
        bitdata::bits length_b = bitdata::to_bits(this->code_lengths[i]);
        coded_data.insert(coded_data.end(), std::make_move_iterator(length_b.begin()), std::make_move_iterator(length_b.end()));
    }

    for (std::size_t i = 0; i < this->code_lengths.size(); ++i) {
        bitdata::bits letter_b = bitdata::to_bits(this->code_letters[i]);
        coded_data.insert(coded_data.end(), std::make_move_iterator(letter_b.begin()), std::make_move_iterator(letter_b.end()));
    }

    for (std::size_t i = 0; i < data.size(); ++i) {
        std::vector<bool> &code_ref = this->code_book[data[i]];
        coded_data.insert(coded_data.end(), std::make_move_iterator(code_ref.begin()), std::make_move_iterator(code_ref.end()));
    }

    std::vector<bool> &code_ref = this->code_book[256];
    coded_data.insert(coded_data.end(), std::make_move_iterator(code_ref.begin()), std::make_move_iterator(code_ref.end()));

    return coded_data;
}


std::vector<int16_t> Huffman::decode(bitdata::bits::iterator reading_head) {

    this->max_code_len = bits_to_uint8(reading_head) + 1;

    this->code_lengths.reserve(this->max_code_len);
    std::size_t letters_cnt = 0;
    for (std::size_t i = 0; i < this->max_code_len; ++i) {
        this->code_lengths[i] = bits_to_uint8(reading_head);
        letters_cnt += this->code_lengths[i];
    }

    this->code_letters.reserve(letters_cnt);
    for (std::size_t i = 0; i < letters_cnt; ++i) {
        this->code_letters[i] = bits_to_int16(reading_head);
    }


    std::vector<size_t> first_code(this->max_code_len);
    std::vector<size_t> first_symbol(this->max_code_len);
    std::size_t c = 0;
    std::size_t s = 0;
    for (int i = 0; i < this->max_code_len; ++i) {
        first_code[i] = c;
        first_symbol[i] = s;
        s += this->code_lengths[i];
        c = (c + this->code_lengths[i]) << 1;
    }

    std::vector<int16_t> out;

    uint64_t c = 0;
    std::size_t l = 0;
    int16_t letter = 0;
    while (letter != 256) {
        c = (c << 1) + static_cast<uint64_t>(*reading_head);
        reading_head++;
        if ((c <<1) < first_code[l+1]) {
            letter = this->code_letters[first_symbol[l] + c - first_code[l]];
            if (letter != 256) {
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
