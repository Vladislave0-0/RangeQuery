// #include <fstream>
// #include <iostream>
// #include <random>

// int main() {
//   int N = 0;
//   std::cin >> N;

//   std::mt19937 rng(std::random_device{}());

//   std::ofstream out;
//   out.open("test.dat");
//   if (out.is_open()) {
//     for (int i = 0; i < N; ++i) {
//       if (rand() % 2) {
//         out << "k ";
//         out << rand() % 1000000 << " ";
//       } else {
//         out << "q ";
//         out << rand() % 1000000 << " " << rand() % 1000000 << " ";
//       }
//     }
//   } else {
//     std::cout << "File didn't wroten\n";
//   }

//   out << std::endl;
//   out.close();
// }