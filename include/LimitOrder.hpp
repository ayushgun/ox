#pragma once

#include "Order.hpp"

/// Represents a limit order in an order book.
class LimitOrder : public Order {
 public:
  explicit LimitOrder(int id, MarketSide side, PriceType price, int size);

  /// Returns the visible size, which is the total size, of the limit order.
  /// Behaves identically to LimitOrder::get_size().
  int get_visible_size() const noexcept override;

  /// Fills the limit order with an aggressive order. The return value indicates
  /// the state of the order (filled or unfilled) after the fill attempt.
  OrderStatus fill(Order& aggressive_order) noexcept override;
};
