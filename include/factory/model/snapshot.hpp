#pragma once

#include <cstdint>
#include <model/ids.hpp>
#include <model/time.hpp>
#include <optional>
#include <vector>

namespace factory {

struct buffer_state {
  buffer_id buffer;
  item_id item;
  std::int64_t quantity{0};
};

enum class resource_status { idle, busy, down, off_shift };

struct resource_state {
  resource_id resource;
  resource_status status{resource_status::idle};
  std::optional<job_id> current_job;
};

struct job_state {
  job_id job;
  order_id order;
  step_id next_step;
  buffer_id location;
  std::int64_t quantity{0};
};

struct snapshot {
  time::tick time{0};
  std::uint64_t last_seq{0};
  std::vector<buffer_state> buffers;
  std::vector<resource_state> resources;
  std::vector<job_state> jobs;
};

} // namespace factory
