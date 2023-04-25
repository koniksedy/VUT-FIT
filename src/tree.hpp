#include <cstdint>

#pragma once


struct Tree {
    enum class Node_Type {inner, leaf};
    Node_Type node_type = Node_Type::inner;
    uint8_t data = 0;
    unsigned cnt = 0;
    unsigned depth = 0;
    Tree *left = nullptr;
    Tree *right = nullptr;

    bool operator<(const Tree &t) const {
        if (this->depth < t.depth) {
            return true;
        } else {
            return this->data < t.data;
        }
    }

    bool operator>(const Tree &t) const {
        if (this->depth > t.depth) {
            return true;
        } else {
            return this->data > t.data;
        }
    }

    void calculate_depth();
};

struct Tree_Val_Differencer {
    int operator() (Tree *a, Tree *b) {
        return a->cnt - b->cnt;
    }
};
