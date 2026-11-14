#pragma once

#include <functional>
#include <string>

namespace factory {

enum class id_kind { item, buffer, resource, routing, step, order, job };

template <id_kind Tag> class id {
public:
  static constexpr id_kind kind{Tag};

  id() = default;
  explicit id(std::string val) : _value(std::move(val)) {}

  [[nodiscard]] const std::string &str() const noexcept { return _value; }

  [[nodiscard]] bool empty() const noexcept { return _value.empty(); }

  friend bool operator==(const id &, const id &) = default;
  friend auto operator<=>(const id &, const id &) = default;

private:
  std::string _value;
};

using item_id = id<id_kind::item>;
using buffer_id = id<id_kind::buffer>;
using resource_id = id<id_kind::resource>;
using routing_id = id<id_kind::routing>;
using step_id = id<id_kind::step>;
using order_id = id<id_kind::order>;
using job_id = id<id_kind::job>;

} // namespace factory

template <factory::id_kind Kind> struct std::hash<factory::id<Kind>> {
  std::size_t operator()(const factory::id<Kind> &id) const noexcept {
    return std::hash<std::string>{}(id.str());
  }
};
