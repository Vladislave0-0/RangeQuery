#pragma once

#include <cstddef>

namespace RB_Tree {
enum class Color { red, black };

template <typename KeyTy> class Tree;

template <typename KeyTy = int> struct Node final {
  KeyTy key;
  RB_Tree::Color color = RB_Tree::Color::red;
  std::size_t subtree_size = 1;

  Node<KeyTy> *left = nullptr;
  Node<KeyTy> *right = nullptr;
  Node<KeyTy> *parent = nullptr;

  Node(const KeyTy &k, Node<KeyTy> *p = nullptr) : key(k), parent(p) {}
  Node() = default;
};

template <typename KeyTy> inline std::size_t size(Node<KeyTy> *node) {
  return node ? node->subtree_size : 0;
}

template <typename KeyTy> inline Node<KeyTy> *minNode(Node<KeyTy> *node) {
  while (node && node->left)
    node = node->left;

  return node;
}

template <typename KeyTy> inline Node<KeyTy> *maxNode(Node<KeyTy> *node) {
  while (node && node->right)
    node = node->right;

  return node;
}

template <typename KeyTy> Node<KeyTy> *successor(Node<KeyTy> *node) {
  if (!node)
    return nullptr;

  if (node->right)
    return minNode(node->right);

  auto current = node;
  auto parent = current->parent;

  while (parent && current == parent->right) {
    current = parent;
    parent = parent->parent;
  }

  return parent;
}

template <typename KeyTy> Node<KeyTy> *predecessor(Node<KeyTy> *node) {
  if (!node)
    return nullptr;

  if (node->left)
    return maxNode(node->left);

  auto current = node;
  auto parent = current->parent;

  while (parent && current == parent->left) {
    current = parent;
    parent = parent->parent;
  }

  return parent;
}
} // namespace RB_Tree
