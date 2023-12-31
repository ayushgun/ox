#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>
#include "Order.hpp"

using OrderList = std::vector<std::unique_ptr<Order>>;
using BidMap = std::map<PriceType, OrderList, std::greater<>>;
using AskMap = std::map<PriceType, OrderList, std::less<>>;

template <typename T>
concept OrderMap = requires(T a) {
  {
    a.begin()
  } -> std::same_as<typename std::map<PriceType, OrderList>::iterator>;
  {
    a.end()
  } -> std::same_as<typename std::map<PriceType, OrderList>::iterator>;
};

/// Represents an order book on the exchange, maintaining lists of bid and ask
/// orders.
class OrderBook {
 public:
  OrderBook();

  OrderBook(const OrderBook&) = delete;
  OrderBook& operator=(const OrderBook&) = delete;

  /// Returns a vector of pointers to all bid orders in the order book.
  std::vector<const Order*> get_bids() const noexcept;

  /// Returns a vector of pointers to all ask orders in the order book.
  std::vector<const Order*> get_asks() const noexcept;

  /// Inserts an Order r-value into the order book and attempts to match it with
  /// existing orders. Requires OrderType to be derived from Order.
  template <typename OrderType>
    requires std::is_base_of<Order, OrderType>::value
  void insert(OrderType&& order) {
    insert(order);
  }

  /// Inserts an Order into the order book and attempts to match it with
  /// existing orders. Requires OrderType to be derived from Order.
  template <typename OrderType>
    requires std::is_base_of<Order, OrderType>::value
  void insert(OrderType& order) {
    bool is_bid = order.get_side() == MarketSide::BID;

    OrderList& level_orders =
        is_bid ? bids[order.get_price()] : asks[order.get_price()];

    if (is_bid) {
      match(order, asks);
    } else {
      match(order, bids);
    }

    if (order.get_size() > 0) {
      level_orders.push_back(std::make_unique<OrderType>(order));
    }
  }

 private:
  /// Attempts to match an incoming aggressive order with resting orders on the
  /// opposite side of the book using a FIFO algorithm.
  template <typename OrderMap>
  void match(Order& aggressive_order, OrderMap& side_map) {
    for (auto& [price, orders] : side_map) {
      // If the aggressive order's price is not favorable for a match, then stop
      // matching.
      if (!aggressive_order.fillable(price)) {
        break;
      }

      std::size_t match_from = 0;
      bool readded_orders = true;

      // Repeatedly attempting to fill orders until no more can be matched.
      while (readded_orders) {
        OrderList buffer;

        // Segregate filled and unfilled orders for trivial deletion of filled
        // orders.
        auto itr = orders.begin() + match_from;
        auto logical_end = itr;
        const auto end = orders.end();

        for (; itr != end; ++itr) {
          auto& order_ptr = *itr;
          OrderStatus fill_status = order_ptr->fill(aggressive_order);

          switch (fill_status) {
            case OrderStatus::FILLED:
              continue;
            case OrderStatus::PEAK_FILLED:
              buffer.push_back(std::move(order_ptr));
              continue;
            case OrderStatus::UNFILLED:
              std::iter_swap(itr, logical_end);
              ++logical_end;
          }
        }

        // Removing fully filled orders from the main list
        orders.erase(logical_end, orders.end());
        readded_orders = !buffer.empty();

        // Re-add peak filled orders to the end for subsequent matching
        [[likely]] if (readded_orders) {
          match_from = orders.size();
          orders.insert(orders.end(), std::make_move_iterator(buffer.begin()),
                        std::make_move_iterator(buffer.end()));
        }
      }
    }
  }

  BidMap bids;
  AskMap asks;
};
