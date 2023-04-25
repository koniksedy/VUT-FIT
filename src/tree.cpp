#include "tree.hpp"

void Tree::calculate_codes() {
    if (this->node_type == Tree::Node_Type::leave) {
        return;
    }

    this->left->depth = this->depth + 1;
    this->left->calculate_codes();
    this->right->depth = this->depth + 1;
    this->right->calculate_codes();
}