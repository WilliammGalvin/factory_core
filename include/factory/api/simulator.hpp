#pragma once

#include <cstdint>
#include <memory>
#include <model/commands.hpp>
#include <model/factory_def.hpp>
#include <model/kpi.hpp>
#include <model/log.hpp>
#include <model/report.hpp>
#include <model/scenario.hpp>
#include <model/snapshot.hpp>
#include <model/time.hpp>
#include <span>
#include <vector>

namespace factory {

[[nodiscard]] std::vector<diagnostic> validate(const factory_def &model);

[[nodiscard]] std::vector<diagnostic> validate(const factory_def &model,
                                               const scenario &s);

[[nodiscard]] bool has_errors(std::span<const diagnostic> diagnostics);

class event_sink {
public:
  virtual ~event_sink() = default;
  virtual void on_record(const log_record &record) = 0;
};

struct run_options {
  time::duration snapshot_interval{0};
  bool keep_log{true};
  bool compute_kpis{true};
  event_sink *sink{nullptr};
};

[[nodiscard]] run_report run(const factory_def &model, const scenario &s,
                             const run_options &opts = {});

struct replication_options {
  int replications{1};
  std::uint64_t base_seed{1};
  unsigned threads{0};
  run_options run;
};

[[nodiscard]] std::vector<run_report>
run_replications(const factory_def &model, const scenario &s,
                 const replication_options &opts);

[[nodiscard]] kpi_report compute_kpis(const factory_def &model,
                                      std::span<const log_record> log,
                                      time::tick window_start,
                                      time::tick window_end);

class simulation {
public:
  simulation(const factory_def &model, const scenario &s,
             const run_options &opts = {});

  ~simulation();

  simulation(simulation &&) noexcept;
  simulation &operator=(simulation &&) noexcept;

  simulation(const simulation &) = delete;
  simulation &operator=(const simulation &) = delete;

  [[nodiscard]] time::tick now() const;
  [[nodiscard]] bool finished() const;

  bool step();

  void run_until(time::tick t);

  void run_to_end();

  void inject(const command &cmd);

  [[nodiscard]] std::vector<log_record> drain_log();

  [[nodiscard]] snapshot snap() const;

  [[nodiscard]] run_report finish();

private:
  struct impl;
  std::unique_ptr<impl> _impl;
};

} // namespace factory
