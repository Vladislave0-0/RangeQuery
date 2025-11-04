#pragma once
#include <cstddef>
#include <list>
#include <optional>

namespace RB_Tree {

enum class Color { red, black };

template <typename KeyTy> struct Node final {
  using It = typename std::list<Node<KeyTy>>::iterator;

  KeyTy key;
  Color color = Color::red;
  std::optional<It> parent = std::nullopt;
  std::optional<It> left = std::nullopt;
  std::optional<It> right = std::nullopt;
  std::size_t subtree_size = 1;

  Node(const KeyTy &key) : key(key) {}
};

template <typename KeyTy>
inline std::size_t size(std::optional<typename Node<KeyTy>::It> node_opt) {
  return node_opt ? (*node_opt)->subtree_size : 0;
}

} // namespace RB_Tree
