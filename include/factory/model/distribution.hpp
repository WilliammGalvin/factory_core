#pragma once

#include <variant>
#include <vector>

namespace factory {

struct constant {
  double value{0.0};
};

struct uniform {
  double lo{0.0};
  double hi{0.0};
};

struct exponential {
  double mean{1.0};
};

struct normal {
  double mean{0.0};
  double stddev{1.0};
  double min{0.0};
};

struct triangular {
  double lo{0.0};
  double hi{0.0};
  double mode{0.0};
};

struct log_normal {
  double mu{0.0};
  double sigma{1.0};
};

struct empirical {
  std::vector<double> samples;
};

using distribution = std::variant<constant, uniform, exponential, normal,
                                  triangular, log_normal, empirical>;

} // namespace factory
