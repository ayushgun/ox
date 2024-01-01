#include "Parser.hpp"
#include <istream>
#include <sstream>
#include <vector>
#include "IcebergOrder.hpp"
#include "LimitOrder.hpp"
#include "Order.hpp"

OrderParser::OrderParser(OrderBook& book, std::istream& input_stream)
    : book(book), input_stream(input_stream) {}

bool OrderParser::ingest() {
  std::string line;

  // Attempt to read a line from the input stream.
  if (std::getline(input_stream, line)) {
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
      return false;
    }

    char type = tokens[0][0];
    int id = std::stoi(tokens[1]);
    int price = std::stoi(tokens[2]);
    int size = std::stoi(tokens[3]);
    MarketSide side = (type == 'B') ? MarketSide::BID : MarketSide::ASK;

    // Determine the order type by the number of tokens and insert the generated
    // Order object into the book.
    if (tokens.size() == 5) {
      int peak_size = std::stoi(tokens[4]);
      book.insert(IcebergOrder(id, side, static_cast<PriceType>(price), size,
                               peak_size));
    } else {
      book.insert(LimitOrder(id, side, static_cast<PriceType>(price), size));
    }

    return true;
  }

  return false;
}
