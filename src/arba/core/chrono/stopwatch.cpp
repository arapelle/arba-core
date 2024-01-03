#include <arba/core/chrono/stopwatch.hpp>

inline namespace arba
{
namespace core
{
inline namespace chrono
{

basic_stopwatch::basic_stopwatch() : start_(clock_type::now())
{
}

stopwatch::duration basic_stopwatch::restart()
{
    time_point previous = start_;
    start_ = clock_type::now();
    return start_ - previous;
}

stopwatch::stopwatch()
{
}

stopwatch::duration stopwatch::elapsed() const
{
    if (is_running()) [[likely]]
        return clock_type::now() - start_;
    return current_ - start_;
}

stopwatch::duration stopwatch::restart()
{
    time_point previous = start_;
    time_point current = current_;
    start_ = clock_type::now();
    resume();
    if (current == time_point()) [[likely]]
        return start_ - previous;
    return current - previous;
}

}
}
}
