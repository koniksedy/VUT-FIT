#include <memory>
#include <cstdint>

#pragma once


struct Tree {
    enum class Node_Type {inner, leaf};
    Node_Type node_type = Node_Type::inner;
    int16_t data = 0;
    unsigned cnt = 0;
    unsigned depth = 0;
    std::shared_ptr<Tree> left = nullptr;
    std::shared_ptr<Tree> right = nullptr;

    inline bool operator<(const Tree &t) const {
        if (this->depth < t.depth) {
            return true;
        }

        if (this->depth == t.depth) {
            return this->data < t.data;
        }

        return false;
    }

    inline bool operator>(const Tree &t) const {
        if (this->depth > t.depth) {
            return true;
        }

        if (this->depth == t.depth) {
            return this->data > t.data;
        }

        return false;
    }

    void calculate_depth();
};

struct Tree_Cnt_Comparator {
    bool operator() (std::shared_ptr<Tree> a, std::shared_ptr<Tree> b) {
        return a->cnt > b->cnt;
    }
};
