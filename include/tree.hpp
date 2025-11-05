#pragma once

#include "node.hpp"

namespace RB_Tree {
template <typename KeyTy = int> class Tree final {
  using NodeTy = Node<KeyTy>;
  using It = typename std::list<NodeTy>::iterator;

  std::optional<It> root_ = std::nullopt;
  std::list<NodeTy> nodes_;

public:
  Tree() = default;
  ~Tree() = default;

  Tree(const Tree &) = delete;
  Tree &operator=(const Tree &) = delete;
  Tree(Tree &&) = default;
  Tree &operator=(Tree &&) = default;

  auto get_root() const { return root_; }
  auto get_nodes() const { return nodes_; }
  bool verifyTree() const;

  void insert(const KeyTy &key);
  std::optional<It> lowerBound(const KeyTy &key) const;
  std::optional<It> upperBound(const KeyTy &key) const;
  std::size_t getRank(std::optional<It> node_opt) const;
  std::size_t distance(std::optional<It> first_opt,
                       std::optional<It> last_opt) const;

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
    auto &node = **current;
    if (node.key >= key) {
      candidate = current;
      current = node.left;
    } else {
      current = node.right;
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
    auto &node = **current;
    if (key < node.key) {
      candidate = current;
      current = node.left;
    } else {
      current = node.right;
    }
  }

  return candidate;
}

template <typename KeyTy>
std::size_t
Tree<KeyTy>::getRank(std::optional<typename Tree<KeyTy>::It> node_opt) const {
  if (!root_ || !node_opt)
    return 0;

  auto node = *node_opt;
  std::size_t rank = size<KeyTy>(node->left);
  auto current = node;

  while (true) {
    auto parent_opt = current->parent;
    if (!parent_opt)
      break;

    auto parent = *parent_opt;
    if (current == parent->right)
      rank += 1 + size<KeyTy>(parent->left);

    current = parent;
  }

  return rank;
}

template <typename KeyTy>
std::size_t
Tree<KeyTy>::distance(std::optional<typename Tree<KeyTy>::It> first_opt,
                      std::optional<typename Tree<KeyTy>::It> last_opt) const {
  if (!root_ || !first_opt)
    return 0;

  if (first_opt == last_opt)
    return 0;

  if (!last_opt)
    return (*root_)->subtree_size - getRank(first_opt);

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
    auto &current_node = *current;

    if (key < current_node.key) {
      if (current_node.left)
        current = *current_node.left;
      else
        break;
    } else if (key > current_node.key) {
      if (current_node.right)
        current = *current_node.right;
      else
        break;
    } else {
      return;
    }
  }

  nodes_.emplace_back(key);
  It new_node = std::prev(nodes_.end());
  new_node->parent = parent;

  auto &parent_node = **parent;
  if (key < parent_node.key)
    parent_node.left = new_node;
  else
    parent_node.right = new_node;

  // Updating the sizes for all nodes.
  auto tmp = parent;
  while (tmp) {
    auto &tmp_node = **tmp;
    ++tmp_node.subtree_size;
    tmp = tmp_node.parent;
  }

  // Balancing after insertion.
  balanceTree(new_node);

  // Updating the root.
  (*root_)->color = Color::black;
}

template <typename KeyTy> void Tree<KeyTy>::balanceTree(It node) {
  while (true) {
    // Get the parent of the current node.
    auto parent_opt = node->parent;

    // If a node does not have a parent, it means that it is the root, and
    // balancing is completed.
    if (!parent_opt)
      break;

    auto parent = *parent_opt;
    // If the parent is black, the tree is already balanced for that node.
    if (parent->color != Color::red)
      break;

    // Getting a grandparent.
    auto grandparent_opt = parent->parent;

    // If there is no grandparent, the tree is balanced.
    if (!grandparent_opt)
      break;

    auto grandparent = *grandparent_opt;
    // We determine whether the parent is the left descendant of the
    // grandfather.
    bool parent_is_left = (grandparent->left && *grandparent->left == parent);

    // We determine the uncle (the parent's brother) based on whether the parent
    // is on the left.
    auto uncle_opt = parent_is_left ? grandparent->right : grandparent->left;

    // Case 1: Uncle is Red.
    if (uncle_opt && (*uncle_opt)->color == Color::red) {
      parent->color = Color::black;
      (*uncle_opt)->color = Color::black;
      grandparent->color = Color::red;
      node = grandparent;
      continue;
    }

    // Uncle is black or null.
    if (parent_is_left) {
      // Case 2: Uncle is black and the node is right.
      if (parent->right && *parent->right == node) {
        node = parent;
        rotateLeft(node);

        // Updating the parent after the rotation.
        parent_opt = node->parent;
        if (!parent_opt)
          break;
        parent = *parent_opt;
      }

      parent->color = Color::black;
      grandparent->color = Color::red;
      rotateRight(grandparent);
    } else {
      // Case 3: Uncle is black and node is left.
      if (parent->left && *parent->left == node) {
        node = parent;
        rotateRight(node);

        // Updating the parent after the rotation.
        parent_opt = node->parent;
        if (!parent_opt)
          break;
        parent = *parent_opt;
      }

      parent->color = Color::black;
      grandparent->color = Color::red;
      rotateLeft(grandparent);
    }
  }

  if (root_)
    (*root_)->color = Color::black;
}

//     x                     y
//    / \                   / \
//   z   y       -->       x   c
//      / \               / \
//     b   c             z   b
template <typename KeyTy> void Tree<KeyTy>::rotateLeft(It x_it) {
  auto &x = *x_it;
  if (!x.right)
    return;

  It y_it = *x.right;
  auto &y = *y_it;

  x.right = y.left;
  if (y.left)
    (*y.left)->parent = x_it;

  y.parent = x.parent;

  if (!x.parent) {
    root_ = y_it;
  } else {
    auto parent_it = *x.parent;
    auto &parent = *parent_it;
    if (x_it == parent.left)
      parent.left = y_it;
    else
      parent.right = y_it;
  }

  y.left = x_it;
  x.parent = y_it;

  x.subtree_size = size<KeyTy>(x.left) + size<KeyTy>(x.right) + 1;
  y.subtree_size = size<KeyTy>(y.left) + size<KeyTy>(y.right) + 1;
}

//       x                 y
//      / \               / \
//     y   z     -->     b   x
//    / \                   / \
//   b   c                 c   z
template <typename KeyTy> void Tree<KeyTy>::rotateRight(It x_it) {
  auto &x = *x_it;
  if (!x.left)
    return;

  It y_it = *x.left;
  auto &y = *y_it;

  x.left = y.right;
  if (y.right)
    (*y.right)->parent = x_it;

  y.parent = x.parent;

  if (!x.parent) {
    root_ = y_it;
  } else {
    auto parent_it = *x.parent;
    auto &parent = *parent_it;
    if (x_it == parent.right)
      parent.right = y_it;
    else
      parent.left = y_it;
  }

  y.right = x_it;
  x.parent = y_it;

  x.subtree_size = size<KeyTy>(x.left) + size<KeyTy>(x.right) + 1;
  y.subtree_size = size<KeyTy>(y.left) + size<KeyTy>(y.right) + 1;
}
} // namespace RB_Tree
