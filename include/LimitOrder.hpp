#pragma once

#include "Order.hpp"

/// Represents a limit order in an order book.
class LimitOrder : public Order {
 public:
  explicit LimitOrder(int id, MarketSide side, PriceType price, int size);

  /// Returns the visible size, which is the total size, of the limit order.
  /// Behaves identically to LimitOrder::get_size().
  int get_visible_size() const noexcept override;

  /// Determines whether the limit order is fillable at the given price. Checks
  /// if the order's limit price is compatible with the specified price.
  bool fillable(PriceType match_price) const noexcept override;

  /// Fills the limit order with an aggressive order. The return value indicates
  /// the state of the order (filled or unfilled) after the fill attempt.
  OrderStatus fill(Order& aggressive_order) noexcept override;
};
