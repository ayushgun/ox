#include <gtest/gtest.h>
#include <vector>
#include "IcebergOrder.hpp"
#include "LimitOrder.hpp"
#include "Order.hpp"
#include "OrderBook.hpp"

struct SimpleOrder {
  int id;
  int visible_size;
  PriceType price;
};

class BookTest : public ::testing::Test {
 protected:
  OrderBook book;

  void ValidateOrder(const Order* order, const SimpleOrder& expected) {
    ASSERT_EQ(order->get_id(), expected.id);
    ASSERT_EQ(order->get_visible_size(), expected.visible_size);
    ASSERT_EQ(order->get_price(), expected.price);
  }

  void ValidateOrders(const std::vector<const Order*>& actual_orders,
                      const std::vector<SimpleOrder>& expected_orders) {
    ASSERT_EQ(actual_orders.size(), expected_orders.size());
    for (std::size_t i = 0; i < actual_orders.size(); ++i) {
      ValidateOrder(actual_orders[i], expected_orders[i]);
    }
  }
};

TEST_F(BookTest, AggressiveIceberg) {
  book.insert(LimitOrder(1, MarketSide::ASK, 101, 20000));
  book.insert(LimitOrder(2, MarketSide::BID, 99, 50000));
  book.insert(LimitOrder(3, MarketSide::ASK, 100, 10000));
  book.insert(LimitOrder(4, MarketSide::ASK, 100, 7500));
  book.insert(LimitOrder(5, MarketSide::BID, 98, 25500));
  book.insert(IcebergOrder(6, MarketSide::BID, 100, 100000, 10000));

  std::vector<SimpleOrder> expected_bids{
      {6, 10000, 100}, {2, 50000, 99}, {5, 25500, 98}};
  std::vector<SimpleOrder> expected_asks{{1, 20000, 101}};

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}

TEST_F(BookTest, LimitAndAggressiveIceberg) {
  book.insert(LimitOrder(100322, MarketSide::BID, 5103, 7500));
  book.insert(IcebergOrder(100345, MarketSide::ASK, 5103, 100000, 10000));

  std::vector<SimpleOrder> expected_bids;
  std::vector<SimpleOrder> expected_asks{{100345, 10000, 5103}};

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}

TEST_F(BookTest, MultipleIcebergMatches) {
  book.insert(IcebergOrder(100322, MarketSide::BID, 5103, 100000, 10000));
  book.insert(IcebergOrder(100345, MarketSide::ASK, 5103, 100000, 10000));

  ASSERT_EQ(book.get_asks().size(), 0);
  ASSERT_EQ(book.get_bids().size(), 0);
}

TEST_F(BookTest, ExhaustIcebergPeak) {
  book.insert(IcebergOrder(100322, MarketSide::BID, 5103, 100000, 10000));
  book.insert(LimitOrder(100345, MarketSide::ASK, 5103, 5000));
  book.insert(LimitOrder(100346, MarketSide::ASK, 5103, 5000));

  std::vector<SimpleOrder> expected_bids{{100322, 10000, 5103}};
  std::vector<SimpleOrder> expected_asks;

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}

TEST_F(BookTest, RemovingAskPriceLevel) {
  book.insert(LimitOrder(100322, MarketSide::ASK, 5103, 5000));
  book.insert(IcebergOrder(100345, MarketSide::ASK, 5103, 15000, 1000));
  book.insert(LimitOrder(100351, MarketSide::ASK, 5104, 50));
  book.insert(LimitOrder(100346, MarketSide::BID, 5103, 21000));

  std::vector<SimpleOrder> expected_bids{{100346, 1000, 5103}};
  std::vector<SimpleOrder> expected_asks{{100351, 50, 5104}};

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}

TEST_F(BookTest, RemovingBidPriceLevel) {
  book.insert(LimitOrder(100322, MarketSide::BID, 100, 5000));
  book.insert(IcebergOrder(100345, MarketSide::BID, 100, 15000, 1000));
  book.insert(LimitOrder(100351, MarketSide::BID, 99, 50));
  book.insert(IcebergOrder(100346, MarketSide::ASK, 100, 21000, 50));

  std::vector<SimpleOrder> expected_bids{{100351, 50, 99}};
  std::vector<SimpleOrder> expected_asks{{100346, 50, 100}};

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}

TEST_F(BookTest, AggressiveBidIcebergWithLesserTotal) {
  book.insert(LimitOrder(1, MarketSide::ASK, 100, 300));
  book.insert(IcebergOrder(2, MarketSide::BID, 100, 500, 400));

  std::vector<SimpleOrder> expected_bids{{2, 200, 100}};
  std::vector<SimpleOrder> expected_asks;

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}

TEST_F(BookTest, NaturalBehavior) {
  book.insert(LimitOrder(1, MarketSide::ASK, 101, 20000));
  book.insert(LimitOrder(2, MarketSide::BID, 99, 50000));
  book.insert(LimitOrder(3, MarketSide::ASK, 100, 10000));
  book.insert(LimitOrder(4, MarketSide::ASK, 100, 7500));
  book.insert(LimitOrder(5, MarketSide::BID, 98, 25500));
  book.insert(IcebergOrder(6, MarketSide::BID, 100, 100000, 10000));
  book.insert(LimitOrder(7, MarketSide::ASK, 100, 10000));
  book.insert(LimitOrder(8, MarketSide::ASK, 100, 11000));
  book.insert(IcebergOrder(9, MarketSide::BID, 100, 50000, 20000));
  book.insert(LimitOrder(10, MarketSide::ASK, 100, 35000));

  std::vector<SimpleOrder> expected_bids{
      {6, 4000, 100}, {9, 20000, 100}, {2, 50000, 99}, {5, 25500, 98}};
  std::vector<SimpleOrder> expected_asks{{1, 20000, 101}};

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}

TEST_F(BookTest, AggressiveAskIcebergWithLesserTotal) {
  book.insert(LimitOrder(1, MarketSide::BID, 100, 300));
  book.insert(IcebergOrder(2, MarketSide::ASK, 100, 500, 400));

  std::vector<SimpleOrder> expected_bids;
  std::vector<SimpleOrder> expected_asks{{2, 200, 100}};

  ValidateOrders(book.get_bids(), expected_bids);
  ValidateOrders(book.get_asks(), expected_asks);
}
