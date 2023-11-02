#include <arba/core/chrono/stopwatch.hpp>
#include <gtest/gtest.h>
#include <format>
#include <cstdlib>

std::chrono::milliseconds default_sleep_duration(100);
std::chrono::milliseconds lower_threshold_duration(1);
std::chrono::milliseconds upper_threshold_duration(20);

// basic_stopwatch tests

TEST(basic_stopwatch_tests, constructor__no_arg__ok)
{
    core::basic_stopwatch swatch;
}

TEST(basic_stopwatch_tests, elapsed__no_arg__ok)
{
    using namespace std::chrono_literals;

    core::basic_stopwatch swatch;
    std::chrono::milliseconds sleep_duration(default_sleep_duration);
    std::this_thread::sleep_for(sleep_duration);
    core::stopwatch::duration delta_time = swatch.elapsed();
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
}

TEST(basic_stopwatch_tests, restart__no_arg__ok)
{
    using namespace std::chrono_literals;

    core::basic_stopwatch swatch;
    std::chrono::milliseconds sleep_duration(default_sleep_duration);
    std::this_thread::sleep_for(sleep_duration);
    core::stopwatch::duration delta_time = swatch.restart();
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
    std::this_thread::sleep_for(sleep_duration);
    delta_time = swatch.elapsed();
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
}

// stopwatch tests

TEST(stopwatch_tests, constructor__no_arg__ok)
{
    core::stopwatch swatch;
    ASSERT_TRUE(swatch.is_running());
    ASSERT_FALSE(swatch.is_paused());
}

TEST(stopwatch_tests, elapsed__no_arg__ok)
{
    using namespace std::chrono_literals;

    core::stopwatch swatch;
    std::chrono::milliseconds sleep_duration(default_sleep_duration);
    std::this_thread::sleep_for(sleep_duration);
    core::stopwatch::duration delta_time = swatch.elapsed();
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
}

TEST(stopwatch_tests, pause_and_resume__no_arg__ok)
{
    using namespace std::chrono_literals;

    core::stopwatch swatch;
    std::chrono::milliseconds sleep_duration(default_sleep_duration);
    std::this_thread::sleep_for(sleep_duration);
    swatch.pause();
    ASSERT_TRUE(swatch.is_paused());
    ASSERT_FALSE(swatch.is_running());
    std::chrono::milliseconds second_sleep_duration = sleep_duration / 2;
    std::this_thread::sleep_for(second_sleep_duration);
    core::stopwatch::duration delta_time = swatch.elapsed();
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
    swatch.resume();
    delta_time = swatch.elapsed();
    sleep_duration += second_sleep_duration;
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
}

TEST(stopwatch_tests, restart__not_paused__ok)
{
    using namespace std::chrono_literals;

    core::stopwatch swatch;
    std::chrono::milliseconds sleep_duration(default_sleep_duration);
    std::this_thread::sleep_for(sleep_duration);
    core::stopwatch::duration delta_time = swatch.restart();
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
    std::this_thread::sleep_for(sleep_duration);
    delta_time = swatch.elapsed();
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
}

TEST(stopwatch_tests, restart__paused__ok)
{
    using namespace std::chrono_literals;

    core::stopwatch swatch;
    std::chrono::milliseconds sleep_duration(default_sleep_duration);
    std::this_thread::sleep_for(sleep_duration);
    swatch.pause();
    std::this_thread::sleep_for(sleep_duration);
    core::stopwatch::duration delta_time = swatch.restart();
    ASSERT_TRUE(swatch.is_running());
    ASSERT_GT(delta_time, sleep_duration - lower_threshold_duration);
    ASSERT_LT(delta_time, sleep_duration + upper_threshold_duration);
}
