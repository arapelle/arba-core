#pragma once

#include <chrono>

inline namespace arba
{
namespace core
{
inline namespace chrono
{

class basic_stopwatch
{
public:
    using clock_type = std::chrono::steady_clock;
    using time_point = typename clock_type::time_point;
    using duration = typename clock_type::duration;

    basic_stopwatch();

    [[nodiscard]] inline duration elapsed() const { return clock_type::now() - start_; }
    duration restart();

protected:
    time_point start_;
};

class stopwatch : protected basic_stopwatch
{
    using base_ = basic_stopwatch;

public:
    using typename base_::clock_type;
    using typename base_::time_point;
    using typename base_::duration;

    stopwatch();

    [[nodiscard]] duration elapsed() const;
    duration restart();
    inline void pause() { current_ = clock_type::now(); }
    inline void resume() { current_ = time_point(); }
    [[nodiscard]] inline bool is_running() const noexcept { return current_ == time_point(); }
    [[nodiscard]] inline bool is_paused() const noexcept { return current_ != time_point(); }

private:
    time_point current_;
};

}
}
}
