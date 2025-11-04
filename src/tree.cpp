#include "../include/tree.hpp"
#include <iostream>
#include <vector>

#ifdef TIME
#include <chrono>
#endif // TIME

// #ifdef GPAPHVIZ_DUMP
// #include "../include/dump.hpp"
// #include <string>
// #endif // GPAPHVIZ_DUMP

// #include "../include/dump.hpp"
// #include <string>
// #include "../include/verify_tree.hpp"

int main() {
  using KeyTy = int;

  volatile std::size_t benchmark_sink = 0;

  char command = 0;
  int first = 0, second = 0;

  RB_Tree::Tree<KeyTy> tree;

  std::vector<int> keys = {2, 4, 6, 8};
  for (int k : keys) {
    tree.insert(k);
  }

  auto nodes = tree.get_nodes();
  for (auto node : nodes) {
    std::string str = (node.color == RB_Tree::Color::black) ? "black" : "red";
    std::cout << "size: " << node.subtree_size << "\ncolor: " << str << "\n\n";
  }

  // static int dot_num = 1;
  // std::string filename =
  //     "./graphviz_output/after_insert_" + std::to_string(dot_num++) + ".dot";
  // makeGraph(filename, tree);

  // bool verify = tree.verifyTree();

  // if(verify) {
  //   std::cout << "\nGOOD TREE\n\n";
  // } else {
  //   std::cout << "\nBAD TREE\n\n";
  // }

  return 0;

#ifdef TIME
  auto begin = std::chrono::steady_clock::now();
#endif // TIME

  while (true) {
    std::cin >> command;

    switch (command) {
    case 'k': {
      std::cin >> first;
      tree.insert(first);

#ifdef GPAPHVIZ_DUMP
      static int dot_num = 1;
      std::string filename = "./graphviz_output/after_insert_" +
                             std::to_string(dot_num++) + ".dot";
      makeGraph(filename, tree.get_root());
#endif // GPAPHVIZ_DUMP

      command = 0;
      break;
    }
    case 'q': {
      std::cin >> first >> second;

      std::size_t distance = 0;
      if (second > first) {
        // RB_Tree::Node<KeyTy> *start = tree.lowerBound(first);
        // RB_Tree::Node<KeyTy> *fin = tree.upperBound(second);
        // distance = tree.distance(start, fin);

        auto start = tree.lowerBound(first);
        auto fin = tree.upperBound(second);
        distance = tree.distance(start, fin);
      }

#ifdef BENCHMARK
      benchmark_sink = distance;
#else
      std::cout << distance << " ";
#endif

      command = 0;
      break;
    }
    default: {
#ifdef TIME
      auto end = std::chrono::steady_clock::now();
      auto elapsed_ms =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      std::cout << std::format("\n\nTime: {} s\n",
                               static_cast<float>(elapsed_ms.count()) / 1000);
#endif // TIME

      return 0;
    }
    }
  }
}
