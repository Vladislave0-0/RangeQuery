#pragma once

#include "node.hpp"
// #include <iostream>
#include <optional>
// #include <stack>

namespace RB_Tree {
template <typename KeyTy = int> class Tree final {
  using NodeTy = Node<KeyTy>;
  using It = typename std::list<NodeTy>::iterator;

  std::optional<It> root_ = std::nullopt;
  std::list<NodeTy> nodes_;

public:
  Tree() { nodes_.clear(); }
  ~Tree() { nodes_.clear(); };

  Tree(const Tree &) = delete;
  Tree &operator=(const Tree &) = delete;
  Tree(Tree &&) = delete;
  Tree &operator=(Tree &&) = delete;

  auto get_root() const { return root_; }
  auto get_nodes() { return nodes_; }

  void insert(const KeyTy &key);
  std::optional<It> lowerBound(const KeyTy &key) const;
  std::optional<It> upperBound(const KeyTy &key) const;
  std::size_t getRank(std::optional<It> node_opt) const;
  std::size_t distance(std::optional<It> first_opt,
                       std::optional<It> last_opt) const;
  bool verifyTree() const;

private:
  void rotateLeft(It node);
  void rotateRight(It node);
  void balanceTree(It node);

  bool checkRedProperty(std::optional<It> node_opt) const;
  bool checkBlackHeight(std::optional<It> node_opt, int black_count,
                        int &path_black_count) const;
  bool checkBSTProperty(std::optional<It> node_opt, std::optional<It> min,
                        std::optional<It> max) const;
  bool checkParentLinks(std::optional<It> node_opt,
                        std::optional<It> parent_opt) const;
  bool checkSubtreeSizes(std::optional<It> node_opt) const;
};

template <typename KeyTy>
std::optional<typename Tree<KeyTy>::It>
Tree<KeyTy>::lowerBound(const KeyTy &key) const {
  if (!root_)
    return std::nullopt;

  std::optional<It> current = root_;
  std::optional<It> candidate;

  while (current) {
    if ((*(*current)).key >= key) {
      candidate = current;
      current = (*(*current)).left;
    } else {
      current = (*(*current)).right;
    }
  }

  return candidate;
}

template <typename KeyTy>
std::optional<typename Tree<KeyTy>::It>
Tree<KeyTy>::upperBound(const KeyTy &key) const {
  if (!root_)
    return std::nullopt;

  std::optional<It> current = root_;
  std::optional<It> candidate;

  while (current) {
    if (key < (*(*current)).key) {
      candidate = current;
      current = (*(*current)).left;
    } else {
      current = (*(*current)).right;
    }
  }

  return candidate;
}

// === Подсчёт ранга (позиции узла в порядке возрастания) ===
template <typename KeyTy>
std::size_t
Tree<KeyTy>::getRank(std::optional<typename Tree<KeyTy>::It> node_opt) const {
  if (!root_ || !node_opt)
    return 0;

  auto node = *node_opt;
  std::size_t rank = size<KeyTy>((*node).left);
  auto current = node;

  while (true) {
    auto parent_opt = (*current).parent;
    if (!parent_opt)
      break;

    auto parent = *parent_opt;
    if (current == *(*parent).right)
      rank += 1 + size<KeyTy>((*parent).left);

    current = parent;
  }

  return rank;
}

// === Расстояние между двумя итераторами ===
template <typename KeyTy>
std::size_t
Tree<KeyTy>::distance(std::optional<typename Tree<KeyTy>::It> first_opt,
                      std::optional<typename Tree<KeyTy>::It> last_opt) const {
  if (!root_ || !first_opt)
    return 0;

  if (first_opt == last_opt)
    return 0;

  if (!last_opt)
    return (*(*root_)).subtree_size - getRank(first_opt);

  std::size_t r1 = getRank(first_opt);
  std::size_t r2 = getRank(last_opt);

  return (r2 >= r1) ? (r2 - r1) : 0;
}

template <typename KeyTy> void Tree<KeyTy>::insert(const KeyTy &key) {
  if (!root_) {
    nodes_.emplace_back(key);
    root_ = std::prev(nodes_.end());
    (*root_)->color = Color::black;
    return;
  }

  It current = *root_;
  std::optional<It> parent;

  while (true) {
    parent = current;
    if (key < (*current).key) {
      if ((*current).left)
        current = *(*current).left;
      else
        break;
    } else if (key > (*current).key) {
      if ((*current).right)
        current = *(*current).right;
      else
        break;
    } else {
      return;
    }
  }

  nodes_.emplace_back(key);
  It new_node = std::prev(nodes_.end());
  (*new_node).parent = parent;

  if (key < (*(*parent)).key)
    (*(*parent)).left = new_node;
  else
    (*(*parent)).right = new_node;

  // Обновляем размеры
  auto tmp = parent;
  while (tmp) {
    ++(*(*tmp)).subtree_size;
    tmp = (*(*tmp)).parent;
  }

  balanceTree(new_node);
  (*(*root_)).color = Color::black;
}

template <typename KeyTy> void Tree<KeyTy>::balanceTree(It node) {
  while (true) {
    auto parent_opt = (*node).parent;
    if (!parent_opt)
      break;
    auto parent = *parent_opt;

    if ((*parent).color != Color::red)
      break;

    auto grandparent_opt = (*parent).parent;
    if (!grandparent_opt)
      break;

    auto grandparent = *grandparent_opt;
    bool parent_is_left =
        ((*grandparent).left && *(*grandparent).left == parent);

    auto uncle_opt =
        parent_is_left ? (*grandparent).right : (*grandparent).left;

    // --- Case 1: Uncle is red ---
    if (uncle_opt && (*(*uncle_opt)).color == Color::red) {
      (*parent).color = Color::black;
      (*(*uncle_opt)).color = Color::black;
      (*grandparent).color = Color::red;
      node = grandparent;
      continue;
    }

    // --- Case 2 + 3: Uncle is black or null ---
    if (parent_is_left) {
      if ((*parent).right && *(*parent).right == node) {
        node = parent;
        rotateLeft(node);
        parent_opt = (*node).parent;
        if (!parent_opt)
          break;
        parent = *parent_opt;
      }
      (*parent).color = Color::black;
      (*grandparent).color = Color::red;
      rotateRight(grandparent);
    } else {
      if ((*parent).left && *(*parent).left == node) {
        node = parent;
        rotateRight(node);
        parent_opt = (*node).parent;
        if (!parent_opt)
          break;
        parent = *parent_opt;
      }
      (*parent).color = Color::black;
      (*grandparent).color = Color::red;
      rotateLeft(grandparent);
    }
  }

  if (root_)
    (*(*root_)).color = Color::black;
}

//     x                     y
//    / \                   / \
//   z   y       -->       x   c
//      / \               / \
//     b   c             z   b
template <typename KeyTy> void Tree<KeyTy>::rotateLeft(It node) {
  auto &n = *node;
  if (!n.right)
    return;

  It right_child = *n.right;
  auto &r = *right_child;

  n.right = r.left;
  if (r.left)
    (*(*r.left)).parent = node;

  r.parent = n.parent;

  if (!n.parent)
    root_ = right_child;
  else if (node == *(*n.parent)->left)
    (*(*n.parent)).left = right_child;
  else
    (*(*n.parent)).right = right_child;

  r.left = node;
  n.parent = right_child;

  n.subtree_size = size<KeyTy>(n.left) + size<KeyTy>(n.right) + 1;
  r.subtree_size = size<KeyTy>(r.left) + size<KeyTy>(r.right) + 1;
}

//       x                 y
//      / \               / \
//     y   z     -->     b   x
//    / \                   / \
//   b   c                 c   z
template <typename KeyTy> void Tree<KeyTy>::rotateRight(It node) {
  auto &n = *node;
  if (!n.left)
    return;

  It left_child = *n.left;
  auto &l = *left_child;

  n.left = l.right;
  if (l.right)
    (*(*l.right)).parent = node;

  l.parent = n.parent;

  if (!n.parent)
    root_ = left_child;
  else if (node == *(*n.parent)->right)
    (*(*n.parent)).right = left_child;
  else
    (*(*n.parent)).left = left_child;

  l.right = node;
  n.parent = left_child;

  n.subtree_size = size<KeyTy>(n.left) + size<KeyTy>(n.right) + 1;
  l.subtree_size = size<KeyTy>(l.left) + size<KeyTy>(l.right) + 1;
}
} // namespace RB_Tree
