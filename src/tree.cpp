#include "tree.hpp"

void Tree::calculate_depth() {
    if (this->node_type == Tree::Node_Type::leaf) {
        return;
    }

    this->left->depth = this->depth + 1;
    this->left->calculate_depth();
    this->right->depth = this->depth + 1;
    this->right->calculate_depth();
}