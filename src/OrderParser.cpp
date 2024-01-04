#include "OrderParser.hpp"
#include <istream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "IcebergOrder.hpp"
#include "LimitOrder.hpp"
#include "Order.hpp"

OrderParser::OrderParser(OrderBook& book, std::istream& input_stream)
    : book(book), input_stream(input_stream) {}

void OrderParser::ingest() {
  std::string line;

  // Attempt to read a line from the input stream.
  if (!std::getline(input_stream, line)) {
    throw std::runtime_error("Failed to read a line from the input stream.");
  }

  std::istringstream iss(line);
  std::vector<std::string> tokens;
  std::string token;

  // Tokenize the line based on commas, which is the expected delimiter in
  // the input format.
  while (std::getline(iss, token, ',')) {
    tokens.push_back(token);
  }

  // Ensure there are enough tokens for a valid order. Less than 4 tokens
  // implies that there is not enough information for a valid order.
  if (tokens.size() < 4) {
    throw std::runtime_error("Insufficient data for a valid order.");
  }

  char type = tokens[0][0];
  unsigned long id_raw = std::stoul(tokens[1]);
  unsigned long price_raw = std::stoul(tokens[2]);
  unsigned long size_raw = std::stoul(tokens[3]);

  if (id_raw > std::numeric_limits<int>::max() ||
      size_raw > std::numeric_limits<int>::max()) {
    throw std::out_of_range("Numeric value is out of the valid range for int.");
  }

  if (price_raw > std::numeric_limits<PriceType>::max()) {
    throw std::out_of_range(
        "Numeric value is out of the valid range for PriceType.");
  }

  int id = static_cast<int>(id_raw);
  PriceType price = static_cast<PriceType>(price_raw);
  int size = static_cast<int>(size_raw);

  MarketSide side = (type == 'B') ? MarketSide::BID : MarketSide::ASK;

  // Determine the order type by the number of tokens and insert the generated
  // Order object into the book.
  if (tokens.size() == 5) {
    unsigned long peak_size_raw = std::stoul(tokens[4]);

    if (peak_size_raw > std::numeric_limits<int>::max()) {
      throw std::out_of_range(
          "Numeric value is out of the valid range for int.");
    }

    int peak_size = static_cast<int>(peak_size_raw);
    book.insert(IcebergOrder(id, side, price, size, peak_size));
  } else {
    book.insert(LimitOrder(id, side, price, size));
  }
}
