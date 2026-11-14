#include <api/serialize.hpp>

namespace factory {

parse_result<factory_def> parse_factory_def(std::string_view) { return {}; }

parse_result<scenario> parse_scenario(std::string_view) { return {}; }

parse_result<command> parse_command(std::string_view) { return {}; }

std::string to_json(const factory_def &) { return {}; }

std::string to_json(const scenario &) { return {}; }

std::string to_json(const snapshot &) { return {}; }

std::string to_json(const kpi_report &) { return {}; }

std::string to_json(const run_report &) { return {}; }

std::string to_ndjson(std::span<const log_record>) { return {}; }

parse_result<std::vector<log_record>> parse_ndjson(std::string_view) {
  return {};
}

std::string hash_of(const factory_def &) { return {}; }

std::string hash_of(const scenario &) { return {}; }

} // namespace factory
