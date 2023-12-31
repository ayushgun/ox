#include <iostream>
#include "IcebergOrder.hpp"
#include "LimitOrder.hpp"
#include "Order.hpp"
#include "OrderBook.hpp"
#include "Visualizer.hpp"

int main() {
  OrderBook book;
  OrderBookVisualizer visualizer(book);

  book.insert(LimitOrder(1, MarketSide::ASK, 101, 20000));
  book.insert(LimitOrder(2, MarketSide::BID, 99, 50000));
  book.insert(LimitOrder(3, MarketSide::ASK, 100, 10000));
  book.insert(LimitOrder(4, MarketSide::ASK, 100, 7500));
  book.insert(LimitOrder(5, MarketSide::BID, 98, 25500));
  book.insert(IcebergOrder(6, MarketSide::BID, 100, 100000, 10000));

  std::cout << visualizer << '\n';

  return 0;
}
