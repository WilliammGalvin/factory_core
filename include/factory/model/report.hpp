#pragma once

#include <cstdint>
#include <model/kpi.hpp>
#include <model/log.hpp>
#include <model/snapshot.hpp>
#include <model/time.hpp>
#include <string>
#include <vector>

namespace factory {

enum class run_status { completed, aborted, invalid_input };

enum class severity { error, warning, info };

struct diagnostic {
  severity sev{severity::info};
  std::string path;
  std::string message;
};

struct run_metadata {
  std::string schema_version;
  std::string library_version;
  std::string model_hash;
  std::string scenario_hash;
  std::uint64_t seed{0};
  time::tick end_time{0};
  std::uint64_t events_processed{0};
  run_status status{run_status::completed};
};

struct run_report {
  run_metadata meta;
  std::vector<log_record> log;
  std::vector<snapshot> snapshots;
  kpi_report kpis;
  std::vector<diagnostic> diagnostics;
};

} // namespace factory
