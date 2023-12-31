#include <iostream>
#include "IcebergOrder.hpp"
#include "LimitOrder.hpp"
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Parser.hpp"
#include "Visualizer.hpp"

int main() {
  OrderBook book;

  OrderBookVisualizer visual(book);
  OrderParser parser(std::cin);

  while (true) {
    std::cout << "Enter Order: ";
    Order* order_ptr = parser.parse();

    if (!order_ptr) {
      std::cerr << "Invalid order format or command. Please try again.\n";
      continue;
    }

    // Using RTTI to handle different order types dynamically.
    if (auto limit_ptr = dynamic_cast<LimitOrder*>(order_ptr)) {
      book.insert(*limit_ptr);
    } else if (auto iceberg_ptr = dynamic_cast<IcebergOrder*>(order_ptr)) {
      book.insert(*iceberg_ptr);
    }

    std::cout << visual << '\n';
  }

  return 0;
}
