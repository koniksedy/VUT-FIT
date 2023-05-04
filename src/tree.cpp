/**
 * @file tree.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief An implementation of a binary tree.
 * @date 04.05.2023
 */

#include "tree.hpp"


/**
 * @brief Calculates depth of each node in a tree. Marks the depth into the node.
 */
void Tree::calculate_depth() {
    if (this->node_type == Tree::Node_Type::leaf) {
        return;
    }

    this->left->depth = this->depth + 1;
    this->left->calculate_depth();
    this->right->depth = this->depth + 1;
    this->right->calculate_depth();
}
