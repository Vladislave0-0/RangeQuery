#pragma once

#include <iostream>

namespace RB_Tree {
enum class Color { red, black };

template <typename KeyTy> struct Node {
  KeyTy key;
  Color color = Color::red;

  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;

  size_t subtree_size = 1;

  Node(const KeyTy &k, Node *p = nullptr) : key(k), parent(p) {}
  Node() = default;

  static size_t size(Node *node) { return node ? node->subtree_size : 0; }
};

template <typename KeyTy> bool is_red(KeyTy *node) noexcept {
  return node && node->color == Color::red;
}

template <typename KeyTy> bool is_left_child(KeyTy *node) noexcept {
  return node && node->parent && node->parent->left == node;
}

template <typename KeyTy> KeyTy *minimum(KeyTy *node) noexcept {
  while (node && node->left)
    node = node->left;
  return node;
}

template <typename KeyTy> KeyTy *maximum(KeyTy *node) noexcept {
  while (node && node->right)
    node = node->right;
  return node;
}

template <typename KeyTy> KeyTy *successor(KeyTy *node) noexcept {
  if (!node)
    return nullptr;

  if (node->right)
    return minimum(node->right);

  auto current = node;
  auto parent = current->parent;
  while (parent && current == parent->right) {
    current = parent;
    parent = parent->parent;
  }

  return parent;
}

template <typename KeyTy> KeyTy *predecessor(KeyTy *node) noexcept {
  if (!node)
    return nullptr;

  if (node->left)
    return maximum(node->left);

  auto current = node;
  auto parent = current->parent;
  while (parent && current == parent->left) {
    current = parent;
    parent = parent->parent;
  }

  return parent;
}

} // namespace Tree