#pragma once

#include <cstdint>
#include <model/distribution.hpp>
#include <model/ids.hpp>
#include <model/time.hpp>
#include <optional>

namespace factory {

struct order_release {
  order_id id;
  item_id product;
  std::int64_t quantity{1};
  time::tick release_at{0};
  std::optional<time::tick> due;
  int priority{0};
};

struct stochastic_demand {
  item_id product;
  distribution interarrival{exponential{}};
  distribution batch_size{constant{1.0}};
  std::optional<time::duration> lead_time;
};

} // namespace factory
