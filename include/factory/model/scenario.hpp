#pragma once

#include <cstdint>
#include <model/commands.hpp>
#include <model/ids.hpp>
#include <model/orders.hpp>
#include <model/policy.hpp>
#include <model/time.hpp>
#include <string>
#include <vector>

namespace factory {

struct buffer_level {
  buffer_id buffer;
  item_id item;
  std::int64_t quantity{0};
};

struct wip_job {
  job_id job;
  order_id order;
  routing_id routing;
  step_id next_step;
  buffer_id location;
  std::int64_t quantity{1};
};

struct initial_state {
  std::vector<buffer_level> buffers;
  std::vector<wip_job> wip;
};

struct scenario {
  std::string id;
  std::string name;
  std::uint64_t seed{1};
  time::tick horizon{0};
  time::tick warmup{0};
  time::time_unit time_unit{time::time_unit::minute};

  std::vector<order_release> orders;
  std::vector<stochastic_demand> demand;
  initial_state init_state;
  policy_config policy;

  std::vector<scheduled_command> scheduled_commands;
};

} // namespace factory
