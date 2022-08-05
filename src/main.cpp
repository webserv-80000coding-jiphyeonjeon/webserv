#include <iostream>
#include <string>

#include "Parser.hpp"
#include "Request.hpp"
#include "Utilities.hpp"
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
    std::cerr << RED << e.what() << END << std::endl;
    return 1;
  }

  // ************************************************************************ //
  //                              Server Setting                              //
  // ************************************************************************ //

  try {
    Webserv webserv(config);
    webserv.initWebserv();
    webserv.runWebserv();
  } catch (std::exception& e) {
    std::cerr << RED << e.what() << END << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Unknown error" << std::endl;
    return 1;
  }
}

// int main() {
//   Request     req;
//   std::string msg =
//       "POST / HTTP/1.1\r\nHost: localhost:8080\r\nContent-Length: ";

//   msg += "10\r\n\r\nHelloWorld";
//   std::cout << msg << std::endl;

//   try {
//     req.parse(msg);
//     req.print();
//   } catch (Request::RequestException& e) {
//     std::cout << e.what() << " " << e.getStatusCode() << std::endl;
//   }

//   std::cout << std::endl;
//   Request     req2;
//   std::string msg2 =
//       "POST / HTTP/1.1\r\nHost: localhost:8080\r\nTransfer-Encoding: "
//       "chunked\r\n\r\n 5\r\nhello\r\na\r\nHelloWorld\r\n0\r\n"
//       "Date: Sun,06 Nov 1994 08:49:37 GMT\r\n"
//       "Content-MD5: 1B2M2Y8AsgTpgAmY7PhCfg ==\r\n\r\n";
//   try {
//     req2.parse(msg2);
//     req2.print();
//   } catch (Request::RequestException& e) {
//     std::cout << e.what() << " " << e.getStatusCode() << std::endl;
//   }
// }
