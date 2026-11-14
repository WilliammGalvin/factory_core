#pragma once

#include <cstddef>
#include <cstdint>
#include <kernel/block.hpp>
#include <kernel/context.hpp>
#include <kernel/event.hpp>
#include <kernel/event_queue.hpp>
#include <kernel/run_result.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace sim::kernel {

struct kernel_config {
  std::uint64_t tick_event_cap{1'000'000};
  std::size_t queue_reserve{0};
  std::size_t recent_event_capacity{32};

  sim_time end_time{std::numeric_limits<sim_time>::max()};
};

class model_error final : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class kernel final : public services {
public:
  explicit kernel(kernel_config config = {});

  kernel(const kernel &) = delete;
  kernel &operator=(const kernel &) = delete;

  block_id add_block(std::unique_ptr<block> blk);

  void start();

  bool step();

  void run_until(sim_time time);

  void run_to_end();

  bool done() const noexcept { return _stopped.has_value(); }

  [[nodiscard]] run_result finish();

  [[nodiscard]] run_result run();

  void schedule(block_id from, block_id target, sim_time delay, event_kind kind,
                payload data, schedule_options opts) override;

  void cancel(block_id self, channel chan) override;

  void request_stop(block_id from, std::string_view reason) override;

  std::size_t block_count() const noexcept { return _slots.size(); }

  sim_time now() const noexcept override { return _now; }

private:
  struct slot {
    std::unique_ptr<block> instance;
    context ctx;
    std::size_t gen_offset;
    std::size_t channel_count;
  };

  enum class step_status { dispatched, idle, halted };

  kernel_config _config;
  event_queue _queue;
  std::vector<slot> _slots;
  std::vector<generation> _generations;

  sim_time _now{0};
  sequence _next_seq{0};
  std::uint64_t _tick_events{0};

  bool _started{false};
  bool _finished{false};
  bool _failed{false};
  bool _stop_requested{false};
  std::string _stop_detail;

  std::optional<stop_reason> _stopped;
  std::string _halt_detail;
  run_stats _stats;

  std::vector<recent_event> _recent;
  std::size_t _recent_next{0};
  std::size_t _recent_count{0};

  step_status _step(sim_time limit);
  void _halt(stop_reason reason, std::string detail);
  void _fail(std::string message);

  bool _is_stale(const event &e) const noexcept;
  void _record_recent(const event &e) noexcept;
  std::vector<recent_event> _collect_recent() const;
};

} // namespace sim::kernel
