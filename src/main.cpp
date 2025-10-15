#include "../include/tree.hpp"
#include <format>

using namespace RB_Tree;

// #define TIME
#ifdef TIME
#include <chrono>
#endif // TIME

// #define GPAPHVIZ_DUMP
#ifdef GPAPHVIZ_DUMP
#include "../include/dump.hpp"
#include <string>
#endif // GPAPHVIZ_DUMP

int main() {
  using KeyTy = int;

  char command = 0;
  int first = 0, second = 0;

  RB_Tree::Tree<KeyTy> tree;

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
      if (first > second) {
        std::cout << std::format("\nBad input format: {} > {}\n", first,
                                 second);
        return 0;
      }

      Tree<KeyTy>::Iterator start = tree.lowerBound(first);
      Tree<KeyTy>::Iterator fin = tree.upperBound(second);

      std::cout << tree.distance(start, fin) << " ";

      command = 0;
      break;
    }

    default: {
      std::cout << std::endl;

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
