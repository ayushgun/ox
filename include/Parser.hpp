#include <istream>
#include "OrderBook.hpp"

// Parses an input stream of order entries into exchange orders and inserts them
// into the OrderBook.
class OrderParser {
 public:
  explicit OrderParser(OrderBook& book, std::istream& input_stream);

  /// Parses a line in the input stream and inserts it into the
  /// OrderBook. Returns true if successful in parsing and inserting, false if
  /// not.
  bool ingest() const;

 private:
  OrderBook& book;
  std::istream& input_stream;
};
