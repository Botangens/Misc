#include "Timer.hpp"

time_t Time::sex{ 0 };

const time_int64_t Time::start_ticks = (time(0) * CLOCKS_PER_SEC) + (clock() % CLOCKS_PER_SEC);


Timer::Timer()
{
}


Timer::~Timer()
{
}
