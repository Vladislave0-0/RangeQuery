#include <iostream>
#include "../include/tree.hpp"
#include "../include/dump.hpp"

using KeyTy = int;

int main() {

  // char command = 0;
  // int first = 0;
  // int second = 0;

  ARB_Tree::Tree<KeyTy> tree;
  // tree.root = new Node<KeyTy>;
  // tree.root->key = 10;

  // tree.root->left = new Node<KeyTy>;
  // tree.root->right = new Node<KeyTy>;

  // tree.root->left->key = 5;
  // tree.root->right->key = 15;

  
  tree.insert(9);
  tree.insert(-1);
  tree.insert(14);
  tree.insert(60);
  tree.insert(1000);

  make_graph("expression.dot", tree.root);

  // while (true) {
  //   std::cin >> command;
  //   // std::cout << command;

  //   switch (command) {
  //   case 'k': {
  //     std::cin >> first;
  //     // std::cout << first << " ";

  //     // tree.insert(a);

  //     command = 0;
  //     break;
  //   }

  //   case 'q': {
  //     std::cin >> first >> second;
  //     // std::cout << first << " " << second << " ";

  //     command = 0;
  //     break;
  //   }

  //   default: {
  //     std::cout << command << " ";
  //     // std::cout << "\ndefault\n";

  //     return 0;
  //   }
  //   }
  // }
}
