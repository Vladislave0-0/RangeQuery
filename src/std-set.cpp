#include <iostream>
#include <iterator>
#include <set>

#define TIME
#ifdef TIME
#include <chrono>
#endif // TIME

int main() {
  using KeyTy = int;

  char command = 0;
  int first = 0, second = 0;

  std::set<int> set_;

#ifdef TIME
  auto begin = std::chrono::steady_clock::now();
#endif // TIME

  while (true) {
    std::cin >> command;

    switch (command) {
    case 'k': {
      std::cin >> first;
      set_.insert(first);

      command = 0;
      break;
    }

    case 'q': {
      std::cin >> first >> second;
      if (first > second) {
        std::cout << std::format("Bad input format: {} > {}\n", first, second);
        return 0;
      }

      std::set<int>::iterator start = set_.lower_bound(first);
      std::set<int>::iterator fin = set_.upper_bound(second);

      std::cout << std::distance(start, fin) << " ";

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
