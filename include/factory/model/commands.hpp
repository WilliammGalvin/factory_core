#pragma once

#include <model/ids.hpp>
#include <model/orders.hpp>
#include <model/policy.hpp>
#include <model/time.hpp>
#include <variant>

namespace factory {

struct force_breakdown {
  resource_id resource;
  time::duration repair_time{0};
};

struct set_resource_online {
  resource_id source;
  bool online{true};
};

struct release_order {
  order_release order;
};

struct cancel_order {
  order_id order;
};

struct change_policy {
  policy_config policy;
};

using command = std::variant<force_breakdown, set_resource_online,
                             release_order, cancel_order, change_policy>;

struct scheduled_command {
  time::tick at{0};
  command cmd;
};

} // namespace factory
