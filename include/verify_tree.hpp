#pragma once
#include "tree.hpp"
#include <iostream>

namespace RB_Tree {
template <typename KeyTy> class Tree;

template <typename KeyTy> bool Tree<KeyTy>::verifyTree() const {
  if (!root_)
    return true;

  // The root must be black.
  if (root_->color != Color::black) {
    std::cerr << "Violation: Root is not black" << std::endl;
    return false;
  }

  // The red node must have black children.
  if (!checkRedProperty(root_)) {
    std::cerr << "Violation: Red node has red child" << std::endl;
    return false;
  }

  // All paths contain the same number of black nodes.
  int black_count = -1;
  if (!checkBlackHeight(root_, 0, black_count)) {
    std::cerr << "Violation: Black height is not consistent" << std::endl;
    return false;
  }

  // Checking the BST property.
  if (!checkBSTProperty(root_)) {
    std::cerr << "Violation: BST property is broken" << std::endl;
    return false;
  }

  // Checking the correctness of parent links.
  if (!checkParentLinks(root_, nullptr)) {
    std::cerr << "Violation: Parent links are incorrect" << std::endl;
    return false;
  }

  // Checking the correctness of subtree_size.
  if (!checkSubtreeSizes(root_)) {
    std::cerr << "Violation: Subtree sizes are incorrect" << std::endl;
    return false;
  }

  return true;
}

template <typename KeyTy>
bool Tree<KeyTy>::checkRedProperty(Node<KeyTy> *node) const {
  if (!node)
    return true;

  if (node->color == Color::red) {
    if (node->left && node->left->color == Color::red) {
      std::cerr << "Red violation at node " << node->key
                << ": left child is red" << std::endl;
      return false;
    }

    if (node->right && node->right->color == Color::red) {
      std::cerr << "Red violation at node " << node->key
                << ": right child is red" << std::endl;
      return false;
    }
  }

  return checkRedProperty(node->left) && checkRedProperty(node->right);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkBlackHeight(Node<KeyTy> *node, int black_count,
                                   int &path_black_count) const {
  if (!node) {
    if (path_black_count == -1) {
      path_black_count = black_count;
    } else if (black_count != path_black_count) {
      std::cerr << "Black height violation: expected " << path_black_count
                << ", got " << black_count << std::endl;
      return false;
    }

    return true;
  }

  if (node->color == Color::black)
    ++black_count;

  return checkBlackHeight(node->left, black_count, path_black_count) &&
         checkBlackHeight(node->right, black_count, path_black_count);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkBSTProperty(Node<KeyTy> *node, Node<KeyTy> *min,
                                   Node<KeyTy> *max) const {
  if (!node)
    return true;

  if (min && node->key <= min->key) {
    std::cerr << "BST violation at node " << node->key
              << ": should be greater than " << min->key << std::endl;
    return false;
  }

  if (max && node->key >= max->key) {
    std::cerr << "BST violation at node " << node->key
              << ": should be less than " << max->key << std::endl;
    return false;
  }

  return checkBSTProperty(node->left, min, node) &&
         checkBSTProperty(node->right, node, max);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkParentLinks(Node<KeyTy> *node,
                                   const Node<KeyTy> *parent) const {
  if (!node)
    return true;

  if (node->parent != parent) {
    std::cerr << "Parent link violation at node " << node->key
              << ": expected parent " << (parent ? parent->key : -1) << ", got "
              << (node->parent ? node->parent->key : -1) << std::endl;
    return false;
  }

  return checkParentLinks(node->left, node) &&
         checkParentLinks(node->right, node);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkSubtreeSizes(Node<KeyTy> *node) const {
  if (!node)
    return true;

  size_t left_size = node->left ? node->left->subtree_size : 0;
  size_t right_size = node->right ? node->right->subtree_size : 0;
  size_t expected_size = 1 + left_size + right_size;

  if (node->subtree_size != expected_size) {
    std::cerr << "Subtree size violation at node " << node->key << ": expected "
              << expected_size << ", got " << node->subtree_size << std::endl;
    return false;
  }

  return checkSubtreeSizes(node->left) && checkSubtreeSizes(node->right);
}
} // namespace RB_Tree
