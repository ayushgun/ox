#include "LimitOrder.hpp"
#include <algorithm>
#include "Order.hpp"

LimitOrder::LimitOrder(int id, MarketSide side, PriceType price, int size)
    : Order(id, side, price, size) {}

int LimitOrder::get_visible_size() const noexcept {
  return get_size();
}

OrderStatus LimitOrder::fill(Order& aggressive_order) noexcept {
  // Trade size is set to the lesser of aggressive or resting order size to
  // prevent overfilling.
  int trade_size = std::min(aggressive_order.get_size(), get_size());
  set_size(get_size() - trade_size);
  aggressive_order.set_size(aggressive_order.get_size() - trade_size);

  return get_size() == 0 ? OrderStatus::FILLED : OrderStatus::UNFILLED;
}
