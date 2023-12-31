#include "Visualizer.hpp"
#include <algorithm>
#include <iomanip>
#include <locale>
#include <sstream>
#include <vector>
#include "Order.hpp"
#include "OrderBook.hpp"

OrderBookVisualizer::OrderBookVisualizer(const OrderBook& book) : book(book) {}

std::ostream& operator<<(std::ostream& os,
                         const OrderBookVisualizer& visualizer) {
  std::ostringstream oss;
  oss << OrderBookVisualizer::DIVIDER << OrderBookVisualizer::HEADER;

  const auto& bids = visualizer.book.get_bids();
  const auto& asks = visualizer.book.get_asks();
  std::size_t max_size = std::max(bids.size(), asks.size());

  // Iterating up to the maximum size of either bids or asks list ensures that
  // the table includes all orders, even if one list is longer than the other.
  for (std::size_t i = 0; i < max_size; ++i) {
    const Order* bid_order = (i < bids.size()) ? bids[i] : nullptr;
    const Order* ask_order = (i < asks.size()) ? asks[i] : nullptr;

    // For each pair of bid and ask orders, a table row is generated,
    // handling cases where one of the lists may have run out of orders with
    // nullptrs.
    visualizer.add_table_row(oss, bid_order, ask_order);
  }

  oss << OrderBookVisualizer::DIVIDER;
  os << oss.str();
  return os;
}

void OrderBookVisualizer::add_table_row(std::ostringstream& oss,
                                        const Order* bid_order,
                                        const Order* ask_order) const {
  // Ensure consistent numeric formatting for prices.
  oss.imbue(std::locale("en_US.UTF-8"));
  oss << "|";

  if (bid_order) {
    oss << std::right << std::setw(10) << bid_order->get_id() << '|'
        << std::setw(13) << bid_order->get_visible_size() << '|' << std::setw(7)
        << std::fixed << std::setprecision(0) << bid_order->get_price() << '|';
  } else {
    oss << std::setw(33) << "|";
  }

  if (ask_order) {
    oss << std::setw(7) << std::fixed << std::setprecision(0)
        << ask_order->get_price() << '|' << std::setw(13)
        << ask_order->get_visible_size() << '|' << std::right << std::setw(10)
        << ask_order->get_id();
  } else {
    oss << std::setw(33) << "";
  }

  oss << "|\n";
}
