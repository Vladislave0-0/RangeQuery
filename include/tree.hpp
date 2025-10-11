#pragma once

#include "node.hpp"
#include <iterator>

namespace ARB_Tree {

template <typename KeyTy> struct Tree {
  Node<KeyTy> *root = nullptr;
  Node<KeyTy> end;

  Tree() {
    end.color = Color::black;
    end.subtree_size = 0;
  };

  void insert(const KeyTy &key) {
    if (!root) {
      root = new Node(key, &end);
      end.left = root;
      root->color = Color::black;
      return;
    }

    // std::cout << "end: " << &end << std::endl;
    // std::cout << "root->parent: " << root->parent << std::endl;

    Node<KeyTy> *parent = root;
    while (true) {
      if (key == parent->key)
        return; // дубликаты не вставляем

      if (key < parent->key) {
        if (!parent->left) {
          parent->left = new Node(key, parent);
          // Обновляем размеры вверх (до end)
          auto temp = parent;
          while (temp && temp != &end) {
            ++temp->subtree_size;
            // std::cout << temp << std::endl;
            temp = temp->parent;
          }

          // Балансировка
          break;
        }

        parent = parent->left;
      } else {
        if (!parent->right) {
          parent->right = new Node(key, parent);
          auto temp = parent;

          while (temp && temp != &end) {
            ++temp->subtree_size;
            // std::cout << temp << std::endl;
            temp = temp->parent;
          }

          // Балансировка
          break;
        }

        parent = parent->right;
      }
    }

    end.left = root;
    while (end.left && end.left->right) {
      end.left = end.left->right;
    }

    // recalculate_size(root);
  }

  size_t recalculate_size(Node<KeyTy> *node) {
    if (!node)
      return 0;

    node->subtree_size =
        1 + recalculate_size(node->left) + recalculate_size(node->right);
    return node->subtree_size;
  }
};

} // namespace ARB_Tree
