#include <iostream>
#include <string>

#include "Parser.hpp"
#include "Webserv.hpp"
#include "color.hpp"

int main(int argc, char* argv[]) {
  if (argc > 2) {
    std::cout << "Usage: " << argv[0] << " [config/*.conf]" << std::endl;
    return 1;
  }

  // ************************************************************************ //
  //                                 Parsing                                  //
  // ************************************************************************ //

  Config      config;
  std::string filename = (argc == 2) ? argv[1] : "./config/default.conf";

  try {
    Parser parser(filename);
    parser.parse(config);
    config.printConfig();
  } catch (std::exception& e) {
    std::cout << RED << e.what() << END << std::endl;
    return 1;
  }

  // ************************************************************************ //
  //                              Server Setting                              //
  // ************************************************************************ //

  try {
    Webserv webserv;
    webserv.initWebserv(config);
    webserv.runWebserv();
  } catch (std::exception& e) {
    std::cerr << RED << e.what() << END << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Unknown error" << std::endl;
    return 1;
  }
}
