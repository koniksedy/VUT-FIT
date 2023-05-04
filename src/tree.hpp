/**
 * @file tree.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file for a binary tree.
 * @date 04.05.2023
 */

#include <memory>
#include <cstdint>

#pragma once


/**
 * @brief A structure of a binary tree.
 */
struct Tree {
    enum class Node_Type {inner, leaf};         // enum of node types
    Node_Type node_type = Node_Type::inner;     // type of this node
    int16_t data = 0;                           // node data
    unsigned cnt = 0;                           // a cout of identical data
    unsigned depth = 0;                         // a node depth
    std::shared_ptr<Tree> left = nullptr;       // a left child
    std::shared_ptr<Tree> right = nullptr;      // a right child

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

    /**
     * @brief Calculates depth of each node in a tree. Marks the depth into the node.
     */
    void calculate_depth();
};

/**
 * @brief A structure for a node comparator. It is used in a priority queue.
 * The node a goes befor the b if a->cnt < b->cnt.
 */
struct Tree_Cnt_Comparator {
    bool operator() (std::shared_ptr<Tree> a, std::shared_ptr<Tree> b) {
        return a->cnt > b->cnt;
    }
};
