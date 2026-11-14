#include <exception>
#include <kernel/event.hpp>
#include <kernel/kernel.hpp>
#include <kernel/run_result.hpp>
#include <optional>
#include <string>

namespace sim::kernel {

namespace {

template <class Fn> std::optional<std::string> capture_error(Fn &&fn) {
  try {
    fn();
  } catch (const std::exception &e) {
    return std::string{e.what()};
  } catch (...) {
    return std::string{"unknown exception"};
  }

  return std::nullopt;
}

} // namespace

kernel::kernel(kernel_config config) : _config(config) {
  _queue.reserve(_config.queue_reserve);
  _recent.resize(_config.recent_event_capacity);
}

block_id kernel::add_block(std::unique_ptr<block> blk) {
  if (_started)
    throw std::logic_error("kernel::add_block() called after start()");

  if (!blk)
    throw std::invalid_argument("kernel::add_block() called with nullptr");

  const std::size_t channels = blk->channel_count();
  if (channels >= no_channel)
    throw std::invalid_argument(
        "block::channel_count() must be less than no_channel");

  const auto id = static_cast<block_id>(_slots.size());
  const std::size_t offset = _generations.size();
  _generations.resize(offset + channels, generation{0});
  _slots.push_back(slot{std::move(blk), context{*this, id}, offset, channels});
  return id;
}

void kernel::schedule(block_id from, block_id target, sim_time delay,
                      event_kind kind, payload data, schedule_options opts) {
  (void)from;

  if (delay < 0)
    throw model_error("kernel::schedule() called with negative delay");

  if (target >= _slots.size())
    throw model_error("kernel::schedule() called with invalid target block_id");

  if (delay > std::numeric_limits<sim_time>::max() - _now)
    throw model_error("kernel::schedule() delay would overflow sim_time");

  generation gen = 0;
  if (opts.chan != no_channel) {
    const slot &target_slot = _slots[target];
    if (opts.chan >= target_slot.channel_count)
      throw model_error("kernel::schedule() called with invalid channel");

    gen = _generations[target_slot.gen_offset + opts.chan];
  }

  _queue.push(event{
      .time = _now + delay,
      .seq = _next_seq++,
      .data = data,
      .target = target,
      .gen = gen,
      .kind = kind,
      .prio = opts.prio,
      .chan = opts.chan,
  });
}

void kernel::cancel(block_id self, channel chan) {
  const slot &current_slot = _slots[self];
  if (chan >= current_slot.channel_count)
    throw model_error("kernel::cancel() called with invalid channel");

  ++_generations[current_slot.gen_offset + chan];
}

void kernel::request_stop(block_id from, std::string_view reason) {
  (void)from;

  if (_stop_requested)
    return;

  _stop_requested = true;
  _stop_detail = reason;
}

void kernel::start() {
  if (_started)
    return;

  _started = true;
  for (slot &s : _slots) {
    if (auto err = capture_error([&]() { s.instance->init(s.ctx); })) {
      _fail(std::move(*err));
      break;
    }
  }
}

bool kernel::step() {
  start();
  return _step(_config.end_time) == step_status::dispatched;
}

void kernel::run_until(sim_time t) {
  start();

  if (t >= _config.end_time) {
    run_to_end();
    return;
  }

  while (_step(t) == step_status::dispatched) {
  }

  if (!_stopped && t > _now) {
    _now = t;
    _tick_events = 0;
  }
}

void kernel::run_to_end() {
  start();

  for (;;) {
    switch (_step(_config.end_time)) {
    case step_status::dispatched:
      continue;

    case step_status::halted:
      return;

    case step_status::idle:
      if (_queue.empty()) {
        _halt(stop_reason::queue_empty, {});
      } else {
        _now = _config.end_time;
        _halt(stop_reason::end_time, {});
      }

      return;
    }
  }
}

run_result kernel::finish() {
  if (_finished)
    throw std::logic_error("kernel::finish() called more than once");

  start();
  _finished = true;

  if (!_stopped) {
    if (_stop_requested)
      _halt(stop_reason::requested, _stop_detail);
    else
      _halt(stop_reason::requested, "finish() called before the run stopped");
  }

  for (slot &s : _slots) {
    if (auto err = capture_error([&]() { s.instance->finish(s.ctx); }))
      _fail(std::move(*err));
  }

  run_result result;
  result.reason = *_stopped;
  result.detail = _halt_detail;
  if (result.reason == stop_reason::tick_storm ||
      result.reason == stop_reason::model_error)
    result.recent_events = _collect_recent();

  _stats.final_time = _now;
  result.stats = _stats;
  return result;
}

run_result kernel::run() {
  if (_finished)
    throw std::logic_error("kernel::run() called more than once");

  start();
  run_to_end();
  return finish();
}

kernel::step_status kernel::_step(sim_time limit) {
  if (_stopped)
    return step_status::halted;

  if (_stop_requested) {
    _halt(stop_reason::requested, _stop_detail);
    return step_status::halted;
  }

  while (!_queue.empty() && _is_stale(_queue.top())) {
    _queue.pop();
    ++_stats.stale_discarded;
  }

  if (_queue.empty() || _queue.top().time > limit)
    return step_status::idle;

  const event e = _queue.pop();
  if (e.time > _now) {
    _now = e.time;
    _tick_events = 0;
  }

  _record_recent(e);
  if (++_tick_events > _config.tick_event_cap) {
    _halt(stop_reason::tick_storm, "tick event cap exceeded");
    return step_status::halted;
  }

  slot &s = _slots[e.target];
  auto err = capture_error([&]() { s.instance->handle(s.ctx, e); });
  ++_stats.events_dispatched;
  if (err) {
    _fail(std::move(*err));
    return step_status::halted;
  }

  return step_status::dispatched;
}

void kernel::_halt(stop_reason reason, std::string detail) {
  if (_stopped)
    return;

  _stopped = reason;
  _halt_detail = std::move(detail);
}

void kernel::_fail(std::string message) {
  if (_stopped)
    return;

  _failed = true;
  _stopped = stop_reason::model_error;
  _halt_detail = std::move(message);
}

bool kernel::_is_stale(const event &e) const noexcept {
  if (e.chan == no_channel)
    return false;

  return e.gen != _generations[_slots[e.target].gen_offset + e.chan];
}

void kernel::_record_recent(const event &e) noexcept {
  if (_recent.empty())
    return;

  _recent[_recent_next] = recent_event{e.time, e.target, e.kind};
  _recent_next = (_recent_next + 1) % _recent.size();
  if (_recent_count < _recent.size())
    ++_recent_count;
}

std::vector<recent_event> kernel::_collect_recent() const {
  std::vector<recent_event> out;
  out.reserve(_recent_count);

  const std::size_t start = (_recent_next + _recent.size() - _recent_count) %
                            (_recent.empty() ? 1 : _recent.size());
  for (std::size_t i = 0; i < _recent_count; ++i)
    out.push_back(_recent[(start + i) % _recent.size()]);

  return out;
}

} // namespace sim::kernel
