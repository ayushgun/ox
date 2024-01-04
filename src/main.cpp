#include <exception>
#include <iostream>
#include "OrderBook.hpp"
#include "OrderBookVisualizer.hpp"
#include "OrderParser.hpp"

int main() {
  OrderBook book;

  OrderBookVisualizer visual(book);
  OrderParser parser(book, std::cin);

  while (true) {
    std::cout << "Enter Order: ";

    try {
      parser.ingest();
    } catch (std::exception& except) {
      std::cerr << "Invalid order format or command: " << except.what() << '\n';
      continue;
    }

    std::cout << visual << '\n';
  }

  return 0;
}
