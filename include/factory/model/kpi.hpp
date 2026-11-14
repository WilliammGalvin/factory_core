#pragma once

#include <cstdint>
#include <model/ids.hpp>
#include <model/time.hpp>
#include <vector>

namespace factory {

struct product_kpi {
  item_id product;
  std::int64_t completed{0};
  double throughput_per_tick{0.0};
  double mean_cycle_time{0.0};
  double p95_cycle_time{0.0};
};

struct resource_kpi {
  resource_id resource;
  double utilization{0.0};
  double downtime_fraction{0.0};
  std::int64_t jobs_processed{0};
};

struct buffer_kpi {
  buffer_id buffer;
  double mean_level{0.0};
  std::int64_t max_level{0};
  double mean_wait{0.0};
};

struct kpi_report {
  time::tick window_start{0};
  time::tick window_end{0};

  std::vector<product_kpi> products;
  std::vector<resource_kpi> resources;
  std::vector<buffer_kpi> buffers;

  double mean_wip{0.0};
  std::int64_t orders_completed{0};
  std::int64_t orders_late{0};
  std::int64_t stockout_events{0};
};

} // namespace factory
