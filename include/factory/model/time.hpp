#pragma once

#include <cstdint>

namespace factory::time {

using tick = std::int64_t;
using duration = std::int64_t;

enum class time_unit { millisecond, second, minute, hour };

} // namespace factory::time
