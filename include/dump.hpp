#pragma once

#include <fstream>

#include "node.hpp"

namespace {
std::string RED = "\x1B[31m";
std::string GRN = "\x1B[32m";
std::string YEL = "\x1B[33m";
std::string BLU = "\x1B[34m";
std::string MAG = "\x1B[35m";
std::string CYN = "\x1B[36m";
std::string WHT = "\x1B[37m";
std::string RST = "\x1B[0m";
}; // namespace

using namespace ARB_Tree;

template <typename KeyTy> void dump_tree(std::ostream &os, Node<KeyTy> *node) {
  std::string node_color = node->color == Color::black ? "black" : "red";
  std::string line_color = node->color == Color::black ? "red" : "black";
  std::string fontcolor = node->color == Color::black ? "white" : "black";

  os << "\tnode_" << node << " [color = " << line_color
     << ", style = \"filled\", fillcolor = " << node_color
     << ", shape = Mrecord, fontcolor = " << fontcolor << ", label =  \"{{<f1> "
     << node << "} | { <f2> size: " << node->subtree_size
     << " | <f3> key = " << node->key << "}}\"];\n";

  if (node->left) {
    os << "\tnode_" << node << ":<f2>:s -> node_" << node->left << ":<f1>:n;\n";
    dump_tree(os, node->left);
  }

  if (node->right) {
    os << "\tnode_" << node << ":<f3>:s -> node_" << node->right
       << ":<f1>:n;\n";
    dump_tree(os, node->right);
  }

  //   if (node->left)
  //     dump_tree(os, node->left);

  //   if (node->right)
  //     dump_tree(os, node->right);
}

template <typename KeyTy>
void make_graph(const std::string &filename, Node<KeyTy> *node) {
  if (node == nullptr) {
    std::cout << RED << "Failed to make dump of the tree. Root is nullptr.\n"
              << RST;
    return;
  }

  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << RED << "Failed to open file: " << filename << std::endl << RST;
    return;
  }

  file << "digraph tree\n";
  file << "{\n";
  file << "\trankdir = TB;\n";
  file << "\tsplines = false;\n\n";

  if (node) {
    dump_tree(file, node);
  }
  file << "}\n";
}
