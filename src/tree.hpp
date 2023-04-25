#include <cstdint>

#pragma once


struct Tree {
    enum class Node_Type {inner, leave};
    Node_Type node_type;
    uint8_t data;
    unsigned cnt;
    unsigned depth;
    Tree *left;
    Tree *right;

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

    void calculate_codes();
};

struct Tree_Val_Differencer {
    int operator() (Tree *a, Tree *b) {
        return a->cnt - b->cnt;
    }
};
