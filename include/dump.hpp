// #pragma once

// #include <fstream>
// #include <iostream>
// #include <string>

// #include "node.hpp"

// namespace RB_Tree {

// template <typename KeyTy> class Tree;

// namespace {
// constexpr const char *RED = "\x1B[31m";
// constexpr const char *RST = "\x1B[0m";
// } // namespace

// template <typename KeyTy>
// void dumpTree(
//     std::ostream &os,
//     const std::optional<typename std::list<Node<KeyTy>>::iterator> &node_opt,
//     size_t &null_counter) {
//   if (!node_opt)
//     return;

//   const Node<KeyTy> &node = **node_opt;
//   const void *node_addr = static_cast<const void *>(&node);

//   std::string node_color = (node.color == Color::black) ? "black" : "red";
//   std::string line_color = (node.color == Color::black) ? "red" : "black";
//   std::string fontcolor = (node.color == Color::black) ? "white" : "black";

//   os << "\tnode_" << node_addr << " [color = " << line_color
//      << ", style = \"filled\", fillcolor = " << node_color
//      << ", shape = Mrecord, fontcolor = " << fontcolor << ", label = \"{{<f1> "
//      << node_addr << "} | {<f2> size: " << node.subtree_size
//      << " | <f3> key = " << node.key << "}}\"];\n";

//   // Left child
//   if (node.left) {
//     const void *left_addr = static_cast<const void *>(&(**node.left));
//     os << "\tnode_" << node_addr << ":<f2>:s -> node_" << left_addr
//        << ":<f1>:n;\n";
//     dumpTree<KeyTy>(os, node.left, null_counter); // ← явное указание KeyTy
//   } else {
//     os << "\tnode_" << node_addr << ":<f2>:s -> null_" << ++null_counter
//        << ";\n";
//     os << "\tnull_" << null_counter
//        << " [color = red, style = \"filled\", fillcolor = black, "
//        << "shape = Mrecord, fontcolor = white, label = \"{<f1> NULL}\"];\n";
//   }

//   // Right child
//   if (node.right) {
//     const void *right_addr = static_cast<const void *>(&(**node.right));
//     os << "\tnode_" << node_addr << ":<f3>:s -> node_" << right_addr
//        << ":<f1>:n;\n";
//     dumpTree<KeyTy>(os, node.right, null_counter); // ← явное указание KeyTy
//   } else {
//     os << "\tnode_" << node_addr << ":<f3>:s -> null_" << ++null_counter
//        << ";\n";
//     os << "\tnull_" << null_counter
//        << " [color = red, style = \"filled\", fillcolor = black, "
//        << "shape = Mrecord, fontcolor = white, label = \"{<f1> NULL}\"];\n";
//   }
// }

// template <typename KeyTy>
// void makeGraph(const std::string &filename, const Tree<KeyTy> &tree) {
//   auto root_opt = tree.get_root();
//   if (!root_opt) {
//     std::cout << RED
//               << "[makeGraph error] Failed to make dump of the tree. Root is "
//                  "nullptr.\n"
//               << RST;
//     return;
//   }

//   std::ofstream file(filename);
//   if (!file.is_open()) {
//     std::cerr << RED << "[makeGraph error] Failed to open file: " << filename
//               << RST << std::endl;
//     return;
//   }

//   file << "digraph tree {\n";
//   file << "\trankdir = TB;\n";
//   file << "\tsplines = false;\n\n";

//   size_t null_counter = 0;
//   dumpTree<KeyTy>(file, root_opt, null_counter);

//   file << "}\n";
// }

// } // namespace RB_Tree