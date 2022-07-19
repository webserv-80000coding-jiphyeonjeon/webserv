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

  std::cout << argc << std::endl;

  for (int i = 1; i < argc; i++) {
    port = atoi(argv[i]);
    receivers.push_back(Receiver(port));
    receivers[i - 1].bind_and_listen();
  }
  for (int i = 0; i < receivers.size(); i++) {
    std::cout << "Receiver " << i << " is listening" << std::endl;
    receivers[i].run();
  }
  return 0;
}