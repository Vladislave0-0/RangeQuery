#pragma once

#include "node.hpp"
#include <iostream>
#include <stack>

namespace RB_Tree {
template <typename KeyTy = int> class Tree final {
  Node<KeyTy> *root_ = nullptr;

public:
  Tree() = default;
  Tree(const Tree &) = delete;
  Tree &operator=(const Tree &) = delete;
  Tree(Tree &&) = delete;
  Tree &operator=(Tree &&) = delete;
  ~Tree() {
    if (!root_)
      return;

    std::stack<Node<KeyTy> *> nodes;
    nodes.push(root_);

    while (!nodes.empty()) {
      Node<KeyTy> *current = nodes.top();
      nodes.pop();

      if (current->left)
        nodes.push(current->left);
      if (current->right)
        nodes.push(current->right);

      delete current;
    }
  };

  Node<KeyTy> *get_root() { return root_; }

  void insert(const KeyTy &key);
  bool verifyTree() const;

private:
  void rotateLeft(Node<KeyTy> *node);
  void rotateRight(Node<KeyTy> *node);
  void balanceTree(Node<KeyTy> *node);

  bool checkRedProperty(Node<KeyTy> *node) const;
  bool checkBlackHeight(Node<KeyTy> *node, int black_count,
                        int &path_black_count) const;
  bool checkBSTProperty(Node<KeyTy> *node, Node<KeyTy> *min = nullptr,
                        Node<KeyTy> *max = nullptr) const;
  bool checkParentLinks(Node<KeyTy> *node, const Node<KeyTy> *parent) const;
  bool checkSubtreeSizes(Node<KeyTy> *node) const;

public:
  Node<KeyTy> *lowerBound(const KeyTy &key) {
    Node<KeyTy> *current = root_;
    Node<KeyTy> *candidate = nullptr;

    while (current) {
      if (current->key >= key) {
        candidate = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }

    return candidate;
  }

  Node<KeyTy> *upperBound(const KeyTy &key) {
    Node<KeyTy> *current = root_;
    Node<KeyTy> *candidate = nullptr;

    while (current) {
      if (key < current->key) {
        candidate = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }

    return candidate;
  }

  size_t getRank(Node<KeyTy> *node) const {
    if (!root_ || !node)
      return 0;

    size_t rank = (node->left ? node->left->subtree_size : 0);
    Node<KeyTy> *current = node;

    while (current != root_) {
      if (current == current->parent->right) {
        rank += 1 + (current->parent->left ? current->parent->left->subtree_size
                                           : 0);
      }
      current = current->parent;
    }

    return rank;
  }

  size_t distance(Node<KeyTy> *first, Node<KeyTy> *last) const {
    if (!root_ || !first)
      return 0;

    if (first == last)
      return 0;

    if (!last)
      return root_->subtree_size - getRank(first);

    size_t r1 = getRank(first);
    size_t r2 = getRank(last);

    return (r2 >= r1) ? (r2 - r1) : 0;
  }
};

template <typename KeyTy> void Tree<KeyTy>::insert(const KeyTy &key) {
  if (!root_) {
    root_ = new Node<KeyTy>(key, nullptr);
    root_->color = Color::black;
    root_->subtree_size = 1;
    return;
  }

  Node<KeyTy> *parent = root_;
  Node<KeyTy> *new_node = nullptr;

  while (true) {
    if (key == parent->key)
      return;

    if (key < parent->key) {
      if (!parent->left) {
        new_node = parent->left = new Node<KeyTy>(key, parent);
        break;
      }
      parent = parent->left;
    } else {
      if (!parent->right) {
        new_node = parent->right = new Node<KeyTy>(key, parent);
        break;
      }
      parent = parent->right;
    }
  }

  // Updating the sizes for all nodes.
  auto temp = new_node ? new_node->parent : parent;
  while (temp) {
    ++temp->subtree_size;
    temp = temp->parent;
  }

  // Balancing after insertion.
  if (new_node)
    balanceTree(new_node);

  // Updating the root.
  while (root_->parent)
    root_ = root_->parent;
}

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

template <typename KeyTy> void Tree<KeyTy>::balanceTree(Node<KeyTy> *node) {
  while (node != root_ && node->parent->color == Color::red) {
    Node<KeyTy> *parent = node->parent;
    Node<KeyTy> *grandparent = parent->parent;

    if (parent == grandparent->left) {
      // Parent is left.
      Node<KeyTy> *uncle = grandparent->right;

      // Case 1: Uncle is Red.
      if (uncle && uncle->color == Color::red) {
        parent->color = Color::black;
        uncle->color = Color::black;
        grandparent->color = Color::red;
        node = grandparent;
      } else {
        // Case 2: Uncle is black and the node is right.
        if (node == parent->right) {
          node = parent;
          rotateLeft(node);

          // Updating the pointers after rotation.
          parent = node->parent;
          grandparent = parent ? parent->parent : nullptr;
        }

        // Case 3: Uncle is black and node is left.
        if (parent) {
          parent->color = Color::black;
          if (grandparent) {
            grandparent->color = Color::red;
            rotateRight(grandparent);
          }
        }
      }
    } else {
      // Parent is right.
      Node<KeyTy> *uncle = grandparent->left;

      // Case 1: Uncle is Red.
      if (uncle && uncle->color == Color::red) {
        parent->color = Color::black;
        uncle->color = Color::black;
        grandparent->color = Color::red;
        node = grandparent;
      } else {
        // Case 2: Uncle is black and the node is left.
        if (node == parent->left) {
          node = parent;
          rotateRight(node);

          // Updating the pointers after rotation.
          parent = node->parent;
          grandparent = parent ? parent->parent : nullptr;
        }

        // Case 3: Uncle is black and the node is right.
        if (parent) {
          parent->color = Color::black;
          if (grandparent) {
            grandparent->color = Color::red;
            rotateLeft(grandparent);
          }
        }
      }
    }
  }

  // The root is always black.
  root_->color = Color::black;
}

//     x                     y
//    / \                   / \
//   z   y       -->       x   c
//      / \               / \
//     b   c             z   b
template <typename KeyTy> void Tree<KeyTy>::rotateLeft(Node<KeyTy> *node) {
  if (!node || !node->right)
    return;

  Node<KeyTy> *right_child = node->right;
  node->right = right_child->left;

  if (right_child->left)
    right_child->left->parent = node;

  right_child->parent = node->parent;

  if (!node->parent) {
    root_ = right_child;
  } else if (node == node->parent->left) {
    node->parent->left = right_child;
  } else {
    node->parent->right = right_child;
  }

  right_child->left = node;
  node->parent = right_child;

  node->subtree_size = size(node->left) + size(node->right) + 1;
  right_child->subtree_size =
      size(right_child->left) + size(right_child->right) + 1;
}

//       x                 y
//      / \               / \
//     y   z     -->     b   x
//    / \                   / \
//   b   c                 c   z
template <typename KeyTy> void Tree<KeyTy>::rotateRight(Node<KeyTy> *node) {
  if (!node || !node->left)
    return;

  Node<KeyTy> *left_child = node->left;
  node->left = left_child->right;

  if (left_child->right)
    left_child->right->parent = node;

  left_child->parent = node->parent;

  if (!node->parent) {
    root_ = left_child;
  } else if (node == node->parent->right) {
    node->parent->right = left_child;
  } else {
    node->parent->left = left_child;
  }

  left_child->right = node;
  node->parent = left_child;

  node->subtree_size = size(node->left) + size(node->right) + 1;
  left_child->subtree_size =
      size(left_child->left) + size(left_child->right) + 1;
}
} // namespace RB_Tree
