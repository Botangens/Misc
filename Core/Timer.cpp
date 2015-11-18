/*
 * Timer.hpp definitions 
 */ 

#include "Timer.hpp"

// no additional dependencies

// ------------------------------ DeltaTime ---------------------------------

// -------------------------------- Time ------------------------------------

// protected static technical variable for temporal conversions to integer seconds
time_t Time::sex{ 0 };

// protected static constant for storaging approximate program start time
const time_int64_t Time::start_ticks = (time(0) * CLOCKS_PER_SEC) - (clock() % CLOCKS_PER_SEC);

// ------------------------------- Timer ------------------------------------

// protected static constant for the reset of Stop time
const DeltaTime Timer::PrestartT = DeltaTime( -1 );

// default constructor. Does NOT start the timer
Timer::Timer()
{
    StartT = StopT = LastT = ProgramTime();
}

// Start function. Timer starts calculating time if has not been calculating
// before. Adds argument dt to allready calculated time
void Timer::Start(const DeltaTime& dt)
{
    StartT = LastT = ProgramTime();
    StopT = PrestartT;
    CountedT += dt;
}

// Timer stops calculating time. If has been calculating before call - updates
// calculated time before stop. Returns calculated time
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

// if Timer IsOn - updates calculated time. Resets timer. Calculated time is
// set to dt. Returns previously calculated time
DeltaTime Timer::Reset(const DeltaTime& dt)
{
    TempT = ProgramTime();

    if (StopT < StartT)
        CountedT += TempT - LastT;

    StopT = StartT = LastT = TempT;

    TempT = CountedT;
    CountedT = dt;
    return TempT;
}

// if Timer IsOn - updates calculated time. Restarts timer, setting calculated
// time to argument dt. Returns previously calculated time
DeltaTime Timer::Restart(const DeltaTime& dt) 
{
    TempT = ProgramTime();

    if (StopT < StartT)
        CountedT += TempT - LastT;

    StartT = LastT = TempT;
    StopT = PrestartT;

    TempT = CountedT;
    CountedT = dt;
    return TempT;
}

// if Timer IsOn (means Stop < Start) updates calculated time. Returns
// calculated time
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
