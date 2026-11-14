#pragma once

#include <cstdint>
#include <map>
#include <model/ids.hpp>
#include <string>
#include <vector>

namespace factory {

struct reorder_point {
  buffer_id buffer;
  item_id item;
  std::int64_t reorder_at{0};
  std::int64_t order_up_to{0};
};

struct policy_config {
  std::string dispatch_rule{"fifo"};
  std::string lot_sizing{"lot_for_lot"};
  std::vector<reorder_point> reorder_points;
  std::map<std::string, std::string> params;
};

} // namespace factory
