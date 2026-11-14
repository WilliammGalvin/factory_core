#pragma once

#include <model/commands.hpp>
#include <model/factory_def.hpp>
#include <model/kpi.hpp>
#include <model/log.hpp>
#include <model/report.hpp>
#include <model/scenario.hpp>
#include <model/snapshot.hpp>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace factory {

template <class T> struct parse_result {
  std::optional<T> value;
  std::vector<diagnostic> diagnostics;
};

[[nodiscard]] parse_result<factory_def> parse_factory(std::string_view json);
[[nodiscard]] parse_result<scenario> parse_scenario(std::string_view json);
[[nodiscard]] parse_result<command> parse_command(std::string_view json);

[[nodiscard]] std::string to_json(const factory_def &model);
[[nodiscard]] std::string to_json(const scenario &model);
[[nodiscard]] std::string to_json(const snapshot &model);
[[nodiscard]] std::string to_json(const kpi_report &model);
[[nodiscard]] std::string to_json(const run_report &model);

[[nodiscard]] std::string to_ndjson(std::span<const log_record> log);
[[nodiscard]] parse_result<std::vector<log_record>>
parse_ndjson(std::string_view ndjson);

[[nodiscard]] std::string hash_of(const factory_def &model);
[[nodiscard]] std::string hash_of(const scenario &model);

} // namespace factory
