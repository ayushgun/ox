#include <iostream>
#include "OrderBook.hpp"
#include "Parser.hpp"
#include "Visualizer.hpp"

int main() {
  OrderBook book;

  OrderBookVisualizer visual(book);
  OrderParser parser(book, std::cin);

  while (true) {
    std::cout << "Enter Order: ";
    bool successfully_parsed = parser.ingest();

    if (!successfully_parsed) {
      std::cerr << "Invalid order format or command. Please try again.\n";
      continue;
    }

    std::cout << visual << '\n';
  }

  return 0;
}
