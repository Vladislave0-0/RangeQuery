// #pragma once

// #include "node.hpp"

// namespace RB_Tree {
// template <typename KeyTy = int> class Tree final {
//   using NodeTy = Node<KeyTy>;
//   using It = typename std::list<NodeTy>::iterator;

//   std::list<NodeTy> nodes_;
//   std::optional<It> root_;

// public:
//   Tree() = default;
//   ~Tree() = default;
//   Tree(const Tree &) = delete;
//   Tree &operator=(const Tree &) = delete;
//   Tree(Tree &&) = delete;
//   Tree &operator=(Tree &&) = delete;

//   std::optional<It> get_root() const { return root_; }
//   std::optional<It> end() const { return std::nullopt; }

//   void insert(const KeyTy &key);
//   bool verifyTree() const;

//   std::optional<It> lowerBound(const KeyTy &key) const;
//   std::optional<It> upperBound(const KeyTy &key) const;
//   std::size_t getRank(std::optional<It> node_opt) const;
//   std::size_t distance(std::optional<It> first_opt,
//                        std::optional<It> last_opt) const;

// private:
//   void rotateLeft(It node);
//   void rotateRight(It node);
//   void balanceTree(It node);

//   bool checkRedProperty(std::optional<It> node_opt) const;
//   bool checkBlackHeight(std::optional<It> node_opt, int black_count,
//                         int &path_black_count) const;
//   bool checkBSTProperty(std::optional<It> node_opt, std::optional<It> min,
//                         std::optional<It> max) const;
//   bool checkParentLinks(std::optional<It> node_opt,
//                         std::optional<It> parent_opt) const;
//   bool checkSubtreeSizes(std::optional<It> node_opt) const;
// };

// template <typename KeyTy> void Tree<KeyTy>::insert(const KeyTy &key) {
//   if (!root_) {
//     // root_ = new Node<KeyTy>(key, nullptr);
//     // root_->color = Color::black;
//     // root_->subtree_size = 1;
//     nodes_.emplace_back(key);
//     root_ = std::prev(nodes_.end());
//     (*(*root_)).color = Color::black;
//     return;
//   }

//   // Node<KeyTy> *parent = root_;
//   // Node<KeyTy> *new_node = nullptr;

//   It current = *root_;
//   std::optional<It> parent;

//   while (true) {
//     parent = current;

//     // if (key == parent->key)
//     //   return;

//     if (key < (*current).key) {
//       if ((*current).left)
//         current = *(*current).left;
//       else
//         break;
//       // if (!parent->left) {
//       //   new_node = parent->left = new Node<KeyTy>(key, parent);
//       //   break;
//       // }
//       // parent = parent->left;
//     } else if (key > (*current).key) {
//       if ((*current).right)
//         current = *(*current).right;
//       else
//         break;
//       // if (!parent->right) {
//       //   new_node = parent->right = new Node<KeyTy>(key, parent);
//       //   break;
//       // }
//       // parent = parent->right;
//     } else {
//       return;
//     }
//   }

//   // // Updating the sizes for all nodes.
//   // auto temp = new_node ? new_node->parent : parent;
//   // while (temp) {
//   //   ++temp->subtree_size;
//   //   temp = temp->parent;
//   // }

//   // // Balancing after insertion.
//   // if (new_node)
//   //   balanceTree(new_node);

//   // // Updating the root.
//   // while (root_->parent)
//   //   root_ = root_->parent;

//   nodes_.emplace_back(key);
//   It new_node = std::prev(nodes_.end());
//   (*new_node).parent = parent;

//   if (key < (*(*parent)).key)
//     (*(*parent)).left = new_node;
//   else
//     (*(*parent)).right = new_node;

//   // Обновляем размеры
//   auto tmp = parent;
//   while (tmp) {
//     ++(*(*tmp)).subtree_size;
//     tmp = (*(*tmp)).parent;
//   }

//   balanceTree(new_node);
//   (*(*root_)).color = Color::black;
// }

// template <typename KeyTy> void Tree<KeyTy>::balanceTree(It node) {
//   while ((*node).parent && (*(*(*node).parent)).color == Color::red) {
//     auto parent = *(*node).parent;
//     auto grandparent_opt = (*parent).parent;
//     if (!grandparent_opt)
//       break;
//     auto grandparent = *grandparent_opt;

//     if (parent == *(*grandparent).left) {
//       auto uncle_opt = (*grandparent).right;
//       if (uncle_opt && (*(*uncle_opt)).color == Color::red) {
//         (*parent).color = Color::black;
//         (*(*uncle_opt)).color = Color::black;
//         (*grandparent).color = Color::red;
//         node = grandparent;
//       } else {
//         if (node == *(*parent).right) {
//           node = parent;
//           rotateLeft(node);
//           parent = *(*node).parent;
//           grandparent_opt = (*parent).parent;
//           if (!grandparent_opt)
//             break;
//           grandparent = *grandparent_opt;
//         }

//         (*parent).color = Color::black;
//         (*grandparent).color = Color::red;
//         rotateRight(grandparent);
//       }
//     } else {
//       auto uncle_opt = (*grandparent).left;
//       if (uncle_opt && (*(*uncle_opt)).color == Color::red) {
//         (*parent).color = Color::black;
//         (*(*uncle_opt)).color = Color::black;
//         (*grandparent).color = Color::red;
//         node = grandparent;
//       } else {
//         if (node == *(*parent).left) {
//           node = parent;
//           rotateRight(node);
//           parent = *(*node).parent;
//           grandparent_opt = (*parent).parent;
//           if (!grandparent_opt)
//             break;
//           grandparent = *grandparent_opt;
//         }

//         (*parent).color = Color::black;
//         (*grandparent).color = Color::red;
//         rotateLeft(grandparent);
//       }
//     }
//   }
//   (*(*root_)).color = Color::black;
// }

// //     x                     y
// //    / \                   / \
// //   z   y       -->       x   c
// //      / \               / \
// //     b   c             z   b
// template <typename KeyTy> void Tree<KeyTy>::rotateLeft(It node) {
//   auto &n = *node;
//   if (!n.right)
//     return;

//   It right_child = *n.right;
//   auto &r = *right_child;

//   n.right = r.left;
//   if (r.left)
//     (*(*r.left)).parent = node;

//   r.parent = n.parent;

//   if (!n.parent)
//     root_ = right_child;
//   else if (node == *(*n.parent)->left)
//     (*(*n.parent)).left = right_child;
//   else
//     (*(*n.parent)).right = right_child;

//   r.left = node;
//   n.parent = right_child;

//   n.subtree_size = size<KeyTy>(n.left) + size<KeyTy>(n.right) + 1;
//   r.subtree_size = size<KeyTy>(r.left) + size<KeyTy>(r.right) + 1;
// }

// //       x                 y
// //      / \               / \
// //     y   z     -->     b   x
// //    / \                   / \
// //   b   c                 c   z
// template <typename KeyTy> void Tree<KeyTy>::rotateRight(It node) {
//   auto &n = *node;
//   if (!n.left)
//     return;

//   It left_child = *n.left;
//   auto &l = *left_child;

//   n.left = l.right;
//   if (l.right)
//     (*(*l.right)).parent = node;

//   l.parent = n.parent;

//   if (!n.parent)
//     root_ = left_child;
//   else if (node == *(*n.parent)->right)
//     (*(*n.parent)).right = left_child;
//   else
//     (*(*n.parent)).left = left_child;

//   l.right = node;
//   n.parent = left_child;

//   n.subtree_size = size<KeyTy>(n.left) + size<KeyTy>(n.right) + 1;
//   l.subtree_size = size<KeyTy>(l.left) + size<KeyTy>(l.right) + 1;
// }

// template <typename KeyTy>
// std::optional<typename std::list<Node<KeyTy>>::iterator>
// Tree<KeyTy>::lowerBound(const KeyTy &key) const {
//   if (!root_)
//     return std::nullopt;

//   std::optional<It> current = root_;
//   std::optional<It> candidate;

//   while (current) {
//     if ((*(*current)).key >= key) {
//       candidate = current;
//       current = (*(*current)).left;
//     } else {
//       current = (*(*current)).right;
//     }
//   }

//   return candidate;
// }

// template <typename KeyTy>
// std::optional<typename std::list<Node<KeyTy>>::iterator>
// Tree<KeyTy>::upperBound(const KeyTy &key) const {
//   if (!root_)
//     return std::nullopt;

//   std::optional<It> current = root_;
//   std::optional<It> candidate;

//   while (current) {
//     if (key < (*(*current)).key) {
//       candidate = current;
//       current = (*(*current)).left;
//     } else {
//       current = (*(*current)).right;
//     }
//   }

//   return candidate;
// }

// template <typename KeyTy>
// std::size_t Tree<KeyTy>::getRank(std::optional<It> node_opt) const {
//   if (!root_ || !node_opt)
//     return 0;

//   auto node = *node_opt;
//   std::size_t rank = size<KeyTy>((*node).left);
//   auto current = node;

//   while (true) {
//     auto parent_opt = (*current).parent;
//     if (!parent_opt)
//       break;

//     auto parent = *parent_opt;
//     if (current == *(*parent).right)
//       rank += 1 + size<KeyTy>((*parent).left);

//     current = parent;
//   }

//   return rank;
// }

// template <typename KeyTy>
// std::size_t Tree<KeyTy>::distance(std::optional<It> first_opt,
//                                   std::optional<It> last_opt) const {
//   if (!root_ || !first_opt)
//     return 0;

//   if (first_opt == last_opt)
//     return 0;

//   if (!last_opt)
//     return (*(*root_)).subtree_size - getRank(first_opt);

//   std::size_t r1 = getRank(first_opt);
//   std::size_t r2 = getRank(last_opt);

//   return (r2 >= r1) ? (r2 - r1) : 0;
// }
// } // namespace RB_Tree