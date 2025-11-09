#pragma once
#include "tree.hpp"
#include <iostream>

namespace RB_Tree {
template <typename KeyTy> bool Tree<KeyTy>::verifyTree() const {
  if (!root_)
    return true;

  if (!root_.has_value() && !nodes_.empty()) {
    std::cerr << "Violation: list with nodes is not empty but root is null."
              << std::endl;
    return false;
  }

  if (root_.has_value() && nodes_.empty()) {
    std::cerr << "Violation: root is set but list with nodes is empty."
              << std::endl;
    return false;
  }

  // The root must be black.
  if ((*root_)->color != Color::black) {
    std::cerr << "Violation: Root is not black." << std::endl;
    return false;
  }

  // The red node must have black children.
  if (!checkRedProperty(root_)) {
    std::cerr << "Violation: Red node has red child." << std::endl;
    return false;
  }

  // All paths contain the same number of black nodes.
  int expected_black_height = -1;
  if (!checkBlackHeight(root_, 0, expected_black_height)) {
    std::cerr << "Violation: Black height is not consistent." << std::endl;
    return false;
  }

  // Checking the BST property.
  if (!checkBSTProperty(root_, std::nullopt, std::nullopt)) {
    std::cerr << "Violation: BST property is broken." << std::endl;
    return false;
  }

  // Checking the correctness of parent links.
  if (!checkParentLinks(root_, std::nullopt)) {
    std::cerr << "Violation: Parent links are incorrect." << std::endl;
    return false;
  }

  // Checking the correctness of subtree_size.
  if (!checkSubtreeSizes(root_)) {
    std::cerr << "Violation: Subtree sizes are incorrect." << std::endl;
    return false;
  }

  return true;
}

template <typename KeyTy>
bool Tree<KeyTy>::checkRedProperty(std::optional<It> node_opt) const {
  if (!node_opt)
    return true;

  const auto &node = **node_opt;
  if (node.color == Color::red) {
    if (node.left && (*node.left)->color == Color::red)
      return false;
    if (node.right && (*node.right)->color == Color::red)
      return false;
  }

  return checkRedProperty(node.left) && checkRedProperty(node.right);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkBlackHeight(std::optional<It> node_opt, int black_count,
                                   int &path_black_count) const {
  if (!node_opt) {
    if (path_black_count == -1)
      path_black_count = black_count;
    return path_black_count == black_count;
  }

  const auto &node = **node_opt;
  if (node.color == Color::black)
    ++black_count;

  return checkBlackHeight(node.left, black_count, path_black_count) &&
         checkBlackHeight(node.right, black_count, path_black_count);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkBSTProperty(std::optional<It> node_opt,
                                   std::optional<It> min,
                                   std::optional<It> max) const {
  if (!node_opt)
    return true;

  const auto &node = **node_opt;
  if (min && (*min)->key >= node.key)
    return false;
  if (max && (*max)->key <= node.key)
    return false;

  return checkBSTProperty(node.left, min, node_opt) &&
         checkBSTProperty(node.right, node_opt, max);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkParentLinks(std::optional<It> node_opt,
                                   std::optional<It> parent_opt) const {
  if (!node_opt)
    return true;

  const auto &node = **node_opt;
  if (node.parent != parent_opt)
    return false;

  return checkParentLinks(node.left, node_opt) &&
         checkParentLinks(node.right, node_opt);
}

template <typename KeyTy>
bool Tree<KeyTy>::checkSubtreeSizes(std::optional<It> node_opt) const {
  if (!node_opt)
    return true;

  const auto &node = **node_opt;
  std::size_t left_size = node.left ? (*node.left)->subtree_size : 0;
  std::size_t right_size = node.right ? (*node.right)->subtree_size : 0;

  if (node.subtree_size != 1 + left_size + right_size)
    return false;

  return checkSubtreeSizes(node.left) && checkSubtreeSizes(node.right);
}
} // namespace RB_Tree
