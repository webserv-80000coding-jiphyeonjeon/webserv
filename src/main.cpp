#include <iostream>
#include <string>

#include "Scanner.hpp"
#include "color.hpp"

int main(int argc, char* argv[]) {
  if (argc > 2) {
    std::cout << "Usage: " << argv[0] << " [config/*.conf]" << std::endl;
    return 1;
  }

  try {
    std::string filename = (argc == 2) ? argv[1] : "./config/default.conf";
    Scanner     scanner;

    std::cout << scanner.readFile(filename) << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  std::cout << GRN "go next step!" END << std::endl;
}
