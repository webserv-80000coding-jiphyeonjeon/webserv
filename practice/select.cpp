#include <iostream>
#include <vector>

#include "Receiver.hpp"

int main(int argc, char **argv) {
  std::vector<Receiver> receivers;

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
    return 1;
  }

  int port;

  for (int i = 1; i <= argc; i++) {
    std::cout << "port: " << argv[i] << std::endl;
    port = atoi(argv[i]);
    receivers.push_back(Receiver(port));
    receivers[i - 1].run();
  }
  return 0;
}