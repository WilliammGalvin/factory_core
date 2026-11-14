#pragma once

#include <cstdint>
#include <model/distribution.hpp>
#include <model/ids.hpp>
#include <model/time.hpp>
#include <optional>
#include <string>
#include <vector>

namespace factory {

struct item_def {
  item_id id;
  std::string name;
  std::string unit;
  bool is_raw_material{false};
  bool is_finished_good{false};
};

struct bom_line {
  item_id item;
  std::int64_t quantity{1};
};

enum class queue_discipline { fifo, lifo, priority };

struct buffer_def {
  buffer_id id;
  std::string name;
  std::optional<item_id> restricted_to;
  std::optional<std::int64_t> capacity;
  queue_discipline discipline{queue_discipline::fifo};
};

struct shift_window {
  time::tick start{0};
  time::tick end{0};
};

struct calendar {
  time::duration period{0};
  std::vector<shift_window> windows;
};

struct failure_model {
  distribution time_to_failure{exponential{}};
  distribution time_to_repair{exponential{}};
  bool preemptive{true};
};

enum class resource_kind { machine, worker, tool, transporter };

struct resource_def {
  resource_id id;
  std::string name;
  resource_kind kind{resource_kind::machine};
  std::int64_t units{1};
  std::optional<factory::calendar> calendar;
  std::optional<failure_model> failure;
};

struct step_def {
  step_id id;
  std::string name;
  std::vector<resource_id> eligible_resources;
  distribution process_time{constant{}};
  std::optional<distribution> setup_time;
  buffer_id input_buffer;
  buffer_id output_buffer;
  std::vector<bom_line> consumes;
  std::vector<bom_line> produces;
  double yield{1.0};
};

struct routing_def {
  routing_id id;
  item_id product;
  std::vector<step_def> steps;
};

struct transport_def {
  buffer_id from;
  buffer_id to;
  distribution travel_time{constant{}};
  std::optional<resource_id> transporter;
};

struct factory_def {
  std::string schema_version{"1.0"};
  std::string name;
  std::vector<item_def> items;
  std::vector<buffer_def> buffers;
  std::vector<resource_def> resources;
  std::vector<routing_def> routings;
  std::vector<transport_def> transports;
};

} // namespace factory
