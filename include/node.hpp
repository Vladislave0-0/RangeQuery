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
};

template <typename KeyTy> inline std::size_t size(Node<KeyTy> *node) {
  return node ? node->subtree_size : 0;
}
} // namespace RB_Tree
