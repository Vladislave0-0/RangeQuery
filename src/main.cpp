#include "../include/dump.hpp"
#include "../include/tree.hpp"

int main() {

  // char command = 0;
  // int first = 0;
  // int second = 0;

  RB_Tree::Tree<int> tree;

  tree.insert(9);
  tree.insert(-1);
  tree.insert(60);
  tree.insert(14);
  tree.insert(1000);
  tree.insert(7);
  tree.insert(1500);
  tree.insert(-9);
  tree.insert(8);
  tree.insert(164);
  tree.insert(512);
  tree.insert(1);
  tree.insert(3);
  tree.insert(5);
  tree.insert(7);
  tree.insert(9);

  for (int i = 0; i < 100; ++i) {
    tree.insert(i);
  }

  // for (auto it = tree.begin(); it != tree.end(); ++it) {
  // std::cout << *tree.lowerBound(3) << "\n";
  // }
  // std::cout << "\n";
  // for (auto it = tree.end(); it != tree.begin();) {
  //   --it;
  //   std::cout << *it << " ";
  // }

  makeGraph("./output/expression.dot", tree.get_root());

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
