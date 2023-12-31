#include "OrderBook.hpp"
#include <memory>
#include <vector>
#include "Order.hpp"

OrderBook::OrderBook() {}

std::vector<const Order*> OrderBook::get_bids() const noexcept {
  std::vector<const Order*> flattened_bids;

  for (const auto& [_, orders] : bids) {
    for (const auto& order_ptr : orders) {
      flattened_bids.push_back(order_ptr.get());
    }
  }

  return flattened_bids;
}

std::vector<const Order*> OrderBook::get_asks() const noexcept {
  std::vector<const Order*> flattened_asks;

  for (const auto& [_, orders] : asks) {
    for (const auto& order_ptr : orders) {
      flattened_asks.push_back(order_ptr.get());
    }
  }

  return flattened_asks;
}
