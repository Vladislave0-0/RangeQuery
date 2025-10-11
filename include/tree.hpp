#pragma once

#include "node.hpp"
#include <iterator>

namespace RB_Tree {
template <typename KeyTy> struct Tree {
  Node<KeyTy> end;

  Node<KeyTy> *root = nullptr;

  Tree() {
    end.color = Color::black;
    end.subtree_size = 0;
  };

  void insert(const KeyTy &key) {
    if (!root) {
      root = new Node<KeyTy>(key, &end);
      end.left = root;
      root->color = Color::black;
      root->subtree_size = 1;
      return;
    }

    Node<KeyTy> *parent = root;
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

    // Обновляем размеры вверх (до end)
    auto temp = new_node ? new_node->parent : parent;
    while (temp && temp != &end) {
      ++temp->subtree_size;
      temp = temp->parent;
    }

    // Балансировка после вставки
    if (new_node) {
      balanceInsertion(new_node);
    }

    // Обновляем корень
    while (root->parent != &end) {
      root = root->parent;
    }

    // Обновляем максимальный элемент
    end.left = root;
    while (end.left && end.left->right) {
      end.left = end.left->right;
    }
  }

  void balanceInsertion(Node<KeyTy> *node) {
    while (node != root && node->parent->color == Color::red) {
      Node<KeyTy> *parent = node->parent;
      Node<KeyTy> *grandparent = parent->parent;

      if (parent == grandparent->left) {
        Node<KeyTy> *uncle = grandparent->right;

        // Случай 1: дядя красный
        if (uncle && uncle->color == Color::red) {
          parent->color = Color::black;
          uncle->color = Color::black;
          grandparent->color = Color::red;
          node = grandparent;
        } else {
          // Случай 2: дядя черный и узел - правый потомок
          if (node == parent->right) {
            node = parent;
            rotateLeft(node);
            // После вращения обновляем указатели
            parent = node->parent;
            grandparent = parent ? parent->parent : nullptr;
          }

          // Случай 3: дядя черный и узел - левый потомок
          if (parent) {
            parent->color = Color::black;
            if (grandparent) {
              grandparent->color = Color::red;
              rotateRight(grandparent);
            }
          }
        }
      } else {
        // Симметричный случай: родитель - правый потомок
        Node<KeyTy> *uncle = grandparent->left;

        // Случай 1: дядя красный
        if (uncle && uncle->color == Color::red) {
          parent->color = Color::black;
          uncle->color = Color::black;
          grandparent->color = Color::red;
          node = grandparent;
        } else {
          // Случай 2: дядя черный и узел - левый потомок
          if (node == parent->left) {
            node = parent;
            rotateRight(node);
            // После вращения обновляем указатели
            parent = node->parent;
            grandparent = parent ? parent->parent : nullptr;
          }

          // Случай 3: дядя черный и узел - правый потомок
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

    // Корень всегда черный
    root->color = Color::black;
  }

  void rotateLeft(Node<KeyTy> *node) {
    if (!node || !node->right)
      return;

    Node<KeyTy> *right_child = node->right;
    node->right = right_child->left;

    if (right_child->left) {
      right_child->left->parent = node;
    }

    right_child->parent = node->parent;

    if (node->parent == &end) {
      root = right_child;
    } else if (node == node->parent->left) {
      node->parent->left = right_child;
    } else {
      node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;

    recalculateSize(node->parent);
  }

  void rotateRight(Node<KeyTy> *node) {
    if (!node || !node->left)
      return;

    Node<KeyTy> *left_child = node->left;
    node->left = left_child->right;

    if (left_child->right) {
      left_child->right->parent = node;
    }

    left_child->parent = node->parent;

    if (node->parent == &end) {
      root = left_child;
    } else if (node == node->parent->right) {
      node->parent->right = left_child;
    } else {
      node->parent->left = left_child;
    }

    left_child->right = node;
    node->parent = left_child;

    recalculateSize(node->parent);
  }

  static void balanceTree(const Tree<KeyTy> &tree) {}

  static size_t recalculateSize(Node<KeyTy> *node) {
    if (!node)
      return 0;

    node->subtree_size =
        1 + recalculateSize(node->left) + recalculateSize(node->right);
    return node->subtree_size;
  }

  bool verify() const {
    if (!root) {
      return true; // Пустое дерево считается валидным
    }

    // Свойство 2: Корень должен быть черным
    if (root->color != Color::black) {
      std::cerr << "Violation: Root is not black" << std::endl;
      return false;
    }

    // Свойство 4: Красный узел должен иметь черных потомков
    if (!checkRedProperty(root)) {
      std::cerr << "Violation: Red node has red child" << std::endl;
      return false;
    }

    // Свойство 5: Все пути содержат одинаковое количество черных узлов
    int black_count = -1;
    if (!checkBlackHeight(root, 0, black_count)) {
      std::cerr << "Violation: Black height is not consistent" << std::endl;
      return false;
    }

    // Проверка BST свойства
    if (!checkBSTProperty(root)) {
      std::cerr << "Violation: BST property is broken" << std::endl;
      return false;
    }

    // Проверка корректности parent связей
    if (!checkParentLinks(root, &end)) {
      std::cerr << "Violation: Parent links are incorrect" << std::endl;
      return false;
    }

    // Проверка корректности subtree_size
    if (!checkSubtreeSizes(root)) {
      std::cerr << "Violation: Subtree sizes are incorrect" << std::endl;
      return false;
    }

    // Проверка end pointer
    if (!checkEndPointer()) {
      std::cerr << "Violation: End pointer is incorrect" << std::endl;
      return false;
    }

    return true;
  }

private:
  bool checkRedProperty(Node<KeyTy> *node) const {
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

  bool checkBlackHeight(Node<KeyTy> *node, int black_count,
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

  bool checkBSTProperty(Node<KeyTy> *node, Node<KeyTy> *min = nullptr,
                        Node<KeyTy> *max = nullptr) const {
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

  bool checkParentLinks(Node<KeyTy> *node, const Node<KeyTy> *parent) const {
    if (!node)
      return true;

    if (node->parent != parent) {
      std::cerr << "Parent link violation at node " << node->key
                << ": expected parent " << (parent ? parent->key : -1)
                << ", got " << (node->parent ? node->parent->key : -1)
                << std::endl;
      return false;
    }

    return checkParentLinks(node->left, node) &&
           checkParentLinks(node->right, node);
  }

  bool checkSubtreeSizes(Node<KeyTy> *node) const {
    if (!node)
      return true;

    size_t left_size = node->left ? node->left->subtree_size : 0;
    size_t right_size = node->right ? node->right->subtree_size : 0;
    size_t expected_size = 1 + left_size + right_size;

    if (node->subtree_size != expected_size) {
      std::cerr << "Subtree size violation at node " << node->key
                << ": expected " << expected_size << ", got "
                << node->subtree_size << std::endl;
      return false;
    }

    return checkSubtreeSizes(node->left) && checkSubtreeSizes(node->right);
  }

  bool checkEndPointer() const {
    if (!root) {
      return end.left == nullptr;
    }

    Node<KeyTy> *max_node = root;
    while (max_node && max_node->right) {
      max_node = max_node->right;
    }

    if (end.left != max_node) {
      std::cerr << "End pointer violation: expected " << max_node->key
                << ", got " << (end.left ? end.left->key : -1) << std::endl;
      return false;
    }

    return true;
  }
};

} // namespace RB_Tree
