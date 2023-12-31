#include <istream>
#include "Order.hpp"

// Class for parsing an input stream of order entries into exchange orders.
class OrderParser {
 public:
  explicit OrderParser(std::istream& input_stream);

  /// Parses the input stream and either returns a LimitOrder or IcebergOrder as
  /// a pointer to an Order.
  Order* parse() const;

 private:
  std::istream& input_stream;
};
