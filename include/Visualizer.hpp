#pragma once

#include <ostream>
#include <sstream>
#include <string_view>
#include "OrderBook.hpp"

class OrderBookVisualizer {
 public:
  OrderBookVisualizer(const OrderBook& book);

  /// Overloads the output stream operator to enable visual representation of
  /// the OrderBook.
  friend std::ostream& operator<<(std::ostream& os,
                                  const OrderBookVisualizer& visualizer);

 private:
  /// Adds a row to the table visualization representing a pair of bid and ask
  /// orders.
  void add_table_row(std::ostringstream& oss,
                     const Order* bid_order,
                     const Order* ask_order) const;

  const OrderBook& book;
  static constexpr std::string_view HEADER =
      "| BID                            | ASK                            |\n"
      "| Id       | Volume      | Price | Price | Volume      | Id       |\n"
      "+----------+-------------+-------+-------+-------------+----------+\n";
  static constexpr std::string_view DIVIDER =
      "+-----------------------------------------------------------------+\n";
};
