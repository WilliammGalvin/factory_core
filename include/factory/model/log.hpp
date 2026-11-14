#pragma once

#include <cstdint>
#include <model/ids.hpp>
#include <model/time.hpp>
#include <variant>

namespace factory {

struct order_released {
  order_id order;
  item_id product;
  std::int64_t quantity{0};
};

struct order_completed {
  order_id order;
  std::int64_t quantity{0};
  bool late{false};
};

struct order_cancelled {
  order_id order;
};

struct job_created {
  job_id job;
  order_id order;
  std::int64_t quantity{0};
};

struct job_moved {
  job_id job;
  buffer_id from;
  buffer_id to;
};

struct step_started {
  job_id job;
  step_id step;
  resource_id resource;
};

struct step_finished {
  job_id job;
  step_id step;
  resource_id resource;
  std::int64_t good{0};
  std::int64_t scrap{0};
};

struct step_interrupted {
  job_id id;
  step_id step;
  resource_id resource;
};

struct resource_failed {
  resource_id resource;
};

struct resource_repaired {
  resource_id resource;
};

struct resource_shift {
  resource_id resource;
  bool on_shift{true};
};

struct stock_changed {
  buffer_id buffer;
  item_id item;
  std::int64_t delta{0};
  std::int64_t level{0};
};

struct stock_out {
  buffer_id buffer;
  item_id item;
  std::int64_t shortfall{0};
};

using log_payload =
    std::variant<order_released, order_completed, order_cancelled, job_created,
                 job_moved, step_started, step_finished, step_interrupted,
                 resource_failed, resource_repaired, resource_shift,
                 stock_changed, stock_out>;

struct log_record {
  std::uint64_t seq{0};
  time::tick time{0};
  log_payload paylod;
};

} // namespace factory
