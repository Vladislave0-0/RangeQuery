#include <fstream>
#include <iostream>
#include <random>

// 1000
// 10000
// 25000
// 50000
// 100000
// 250000
// 500000
// 1000000

int main() {
  srand(time(nullptr));

  int N = 0;
  std::cin >> N;

  std::mt19937 rng(std::random_device{}());

  std::ofstream out;
  out.open("name.dat");
  if (out.is_open()) {
    for (int i = 0; i < N; ++i) {
      if (rand() % 2) {
        out << "k ";
        out << rand() % 1000000 << " ";
      } else {
        int first = rand() % 1000000;
        int second = rand() % 1000000;

        while (first > second) {
          first = rand() % 1000000;
          second = rand() % 1000000;
        }

        out << "q ";
        out << first << " " << second << " ";
      }
    }
  } else {
    std::cout << "File didn't wroten\n";
  }

  out << std::endl;
  out.close();
}
