#pragma once

#include <iostream>

namespace RB_Tree {
enum class Color { red, black };

template <typename KeyTy> class Tree;

template <typename KeyTy = int> class Node {

  KeyTy key;
  RB_Tree::Color color = RB_Tree::Color::red;
  std::size_t subtree_size = 1;

  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;

public:
  friend class Tree<KeyTy>;

  Node(const KeyTy &k, Node *p = nullptr) : key(k), parent(p) {}
  Node() = default;

  std::size_t size(Node *node) const { return node ? node->subtree_size : 0; }
  Node<KeyTy> *get_left() { return left; }
  Node<KeyTy> *get_right() { return right; }
  Node<KeyTy> *get_parent() { return parent; }
  KeyTy get_key() { return key; }
  RB_Tree::Color get_color() { return color; }
  std::size_t get_subtree_size() { return subtree_size; }
};

template <typename KeyTy> Node<KeyTy> *minNode(Node<KeyTy> *node) {
  while (node && node->left)
    node = node->left;

  return node;
}

template <typename KeyTy> Node<KeyTy> *maxNode(Node<KeyTy> *node) {
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

template <typename KeyTy> Node<KeyTy> *predecessor(KeyTy *node) {
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
