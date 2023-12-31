#include "IcebergOrder.hpp"
#include <algorithm>
#include "Order.hpp"

IcebergOrder::IcebergOrder(int id,
                           MarketSide side,
                           PriceType price,
                           int size,
                           int peak_size)
    : Order(id, side, price, size),
      original_peak_size(peak_size),
      peak_size(peak_size) {}

int IcebergOrder::get_visible_size() const noexcept {
  return std::min(peak_size, get_size());
}

OrderStatus IcebergOrder::fill(Order& aggressive_order) noexcept {
  // The trade size is the minimum of the aggressive order's size and the peak
  // size to ensures that only the visible part of the iceberg order can be
  // filled.
  int trade_size = std::min(aggressive_order.get_size(), peak_size);

  peak_size -= trade_size;
  set_size(get_size() - trade_size);
  aggressive_order.set_size(aggressive_order.get_size() - trade_size);

  if (peak_size == 0) {
    [[unlikely]] if (get_size() == 0) { return OrderStatus::FILLED; }

    // Reset peak size for the next peak, capped at the remaining order size.
    // This avoids showing a larger peak size than what is actually left in the
    // order.
    peak_size = std::min(original_peak_size, get_size());
    return OrderStatus::PEAK_FILLED;
  }

  return OrderStatus::UNFILLED;
}
