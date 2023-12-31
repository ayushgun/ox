#pragma once

#include "Order.hpp"

/// Represents an iceberg order, which partially hides the total order size, in
/// an order book.
class IcebergOrder : public Order {
 public:
  explicit IcebergOrder(int id,
                        MarketSide side,
                        PriceType price,
                        int size,
                        int peak_size);

  /// Returns the currently visible size of the iceberg order, which is either
  /// the peak size or the total quantity if it is smaller than the peak size.
  int get_visible_size() const noexcept override;

  /// Fills the iceberg order with an aggressive order. The return value
  /// indicates the state of the order (filled, peak filled, or unfilled) after
  /// the fill attempt.
  OrderStatus fill(Order& aggressive_order) noexcept override;

 private:
  const int original_peak_size;
  int peak_size;
};
