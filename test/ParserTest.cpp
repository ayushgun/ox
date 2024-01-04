#include <gtest/gtest.h>
#include <sstream>
#include "Order.hpp"
#include "OrderBook.hpp"
#include "OrderParser.hpp"

class ParserTest : public ::testing::Test {
 protected:
  OrderBook book;
  std::stringstream in;
  OrderParser parser;

  ParserTest() : parser(book, in) {}
};

TEST_F(ParserTest, ParseLimitOrder) {
  in << "B,100322,5103,7500";
  parser.ingest();

  ASSERT_EQ(book.get_bids().size(), 1);

  const Order* order = book.get_bids().at(0);
  ASSERT_EQ(order->get_id(), 100322);
  ASSERT_EQ(order->get_price(), 5103);
  ASSERT_EQ(order->get_size(), 7500);
  ASSERT_EQ(order->get_visible_size(), 7500);
}

TEST_F(ParserTest, ParseIcebergOrder) {
  in << "A,100345,5103,100000,10000";
  parser.ingest();

  ASSERT_EQ(book.get_asks().size(), 1);

  const Order* order = book.get_asks().at(0);
  ASSERT_EQ(order->get_id(), 100345);
  ASSERT_EQ(order->get_price(), 5103);
  ASSERT_EQ(order->get_size(), 100000);
  ASSERT_EQ(order->get_visible_size(), 10000);
}
