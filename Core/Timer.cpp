#include "Timer.hpp"

time_t Time::sex{ 0 };

const time_int64_t Time::start_ticks = (time(0) * CLOCKS_PER_SEC) + (clock() % CLOCKS_PER_SEC);


Timer::Timer()
{
    StartT = StopT = LastT = ProgramTime();
}

Timer::~Timer()
{
}

void Timer::Start(const DeltaTime& dt)
{
    StartT = LastT = ProgramTime();
    StopT = MIN_DeltaTime;
    CountedT += dt;
}
DeltaTime Timer::Stop()
{
    TempT = ProgramTime();
    if (StopT < StartT)
    {
        CountedT += TempT - LastT;
        LastT = TempT;
    }
    StopT = TempT;
    return CountedT;
}
DeltaTime Timer::Reset(const DeltaTime& dt)
{
    StopT = StartT = LastT = ProgramTime();
    TempT = CountedT;
    CountedT = dt;
    return TempT;
}
DeltaTime Timer::Restart(const DeltaTime& dt) 
{
    TempT = ProgramTime();

    if (StopT < StartT)
        CountedT += TempT - LastT;

    StartT = LastT = TempT;
    StopT = MIN_DeltaTime;

    TempT = CountedT;
    CountedT = dt;
    return TempT;
}
DeltaTime Timer::Update()
{
    if (StopT < StartT)
    {
        TempT = ProgramTime();
        CountedT += TempT - LastT;
        LastT = TempT;
    }
    return CountedT;
}