#include "Parser.hpp"
#include <istream>
#include <sstream>
#include <vector>
#include "IcebergOrder.hpp"
#include "LimitOrder.hpp"
#include "Order.hpp"

OrderParser::OrderParser(std::istream& input_stream)
    : input_stream(input_stream) {}

Order* OrderParser::parse() const {
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
      return nullptr;
    }

    char type = tokens[0][0];
    int id = std::stoi(tokens[1]);
    int price = std::stoi(tokens[2]);
    int size = std::stoi(tokens[3]);
    MarketSide side = (type == 'B') ? MarketSide::BID : MarketSide::ASK;

    // Determine the order type by the number of tokens.
    if (tokens.size() == 5) {
      int peak_size = std::stoi(tokens[4]);
      return new IcebergOrder(id, side, static_cast<PriceType>(price), size,
                              peak_size);
    } else {
      return new LimitOrder(id, side, static_cast<PriceType>(price), size);
    }
  }

  // Return nullptr if no line was read, indicating end of input or error.
  return nullptr;
}
