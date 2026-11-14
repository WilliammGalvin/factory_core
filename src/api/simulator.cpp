#include <api/simulator.hpp>

namespace factory {

std::vector<diagnostic> validate(const factory_def &model) { return {}; }

std::vector<diagnostic> validate(const factory_def &model, const scenario &s) {
  return {};
}

bool has_errors(std::span<const diagnostic> diagnostics) { return false; }

run_report run(const factory_def &model, const scenario &s,
               const run_options &opts) {
  return {};
}

std::vector<run_report> run_replications(const factory_def &model,
                                         const scenario &s,
                                         const replication_options &opts) {
  return {};
}

kpi_report compute_kpis(const factory_def &model,
                        std::span<const log_record> log,
                        time::tick window_start, time::tick window_end) {
  return {};
}

struct simulation::impl {
  //
};

simulation::simulation(const factory_def &model, const scenario &s,
                       const run_options &opts)
    : _impl(std::make_unique<impl>()) {}

simulation::~simulation() = default;

simulation::simulation(simulation &&) noexcept = default;
simulation &simulation::operator=(simulation &&) noexcept = default;

time::tick simulation::now() const { return 0; }

bool simulation::finished() const { return true; }

bool simulation::step() { return false; }

void simulation::run_until(time::tick t) {
  //
}

void simulation::run_to_end() {
  //
}

void simulation::inject(const command &cmd) {
  //
}

std::vector<log_record> simulation::drain_log() { return {}; }

snapshot simulation::snap() const { return {}; }

run_report simulation::finish() { return {}; }

} // namespace factory
