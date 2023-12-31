#pragma once

#include <cstdint>

enum class MarketSide { BID, ASK };

/// The possible statuses of an order after being filled by an aggressive order.
enum class OrderStatus { FILLED, PEAK_FILLED, UNFILLED };

using PriceType = std::uint16_t;

/// Abstract base class for financial orders, providing common properties and
/// interfaces.
class Order {
 public:
  explicit Order(int id, MarketSide side, PriceType price, int size);

  /// Returns the order ID.
  int get_id() const noexcept;

  /// Returns the market side of the order (BID or ASK).
  MarketSide get_side() const noexcept;

  /// Returns the price of the order.
  PriceType get_price() const noexcept;

  /// Returns the total size of the order.
  int get_size() const noexcept;

  /// Sets a new size for the order.
  void set_size(int new_size) noexcept;

  /// Returns the visible size of the order.
  virtual int get_visible_size() const noexcept = 0;

  /// Determines whether the order is fillable at the given match price.
  virtual bool fillable(PriceType match_price) const noexcept = 0;

  /// Fills the order with an aggressive order.
  virtual OrderStatus fill(Order& aggressive_order) noexcept = 0;

  virtual ~Order() = default;

 private:
  const int id;
  const MarketSide side;
  const PriceType price;
  int size;
};
