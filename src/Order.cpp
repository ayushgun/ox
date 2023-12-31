#include "Order.hpp"

Order::Order(int id, MarketSide side, PriceType price, int size)
    : id(id), side(side), price(price), size(size) {}

int Order::get_id() const noexcept {
  return id;
}

MarketSide Order::get_side() const noexcept {
  return side;
}

PriceType Order::get_price() const noexcept {
  return price;
}

int Order::get_size() const noexcept {
  return size;
}

void Order::set_size(int new_size) noexcept {
  size = new_size;
}
