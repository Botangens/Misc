/*
 * Timer.hpp definitions 
 */ 

#include "Timer.hpp"

// Private constant - program start chrono::time_point
const std::chrono::steady_clock::time_point START_time_point =
    std::chrono::steady_clock::now();

namespace Perspective
{

// ------------------------------ Duration ---------------------------------

    // implementation of main function. Actually must be inlined but I doubt.
    Duration ProgramTime()
    {
      //return Duration( ( std::chrono::steady_clock::now() -
      //    START_time_point ).count() / COUNT_PER_CLOCK );
        return Duration( (std::chrono::steady_clock::now() -
            START_time_point ).count() );
    }

// -------------------------------- Time ------------------------------------

    // protected static technical variable for temporal conversions to integer seconds
    time_t Time::sex{ 0 };

    // protected static constant for storaging approximate program start time
    // TODO: add constatnc Duration::startTicks based on SteadyClock
    const time_tick_t Time::start_ticks = (time(0) * TICKS_PER_SEC) + 
        (ProgramTime().getTicks() % TICKS_PER_SEC);

    // returns current moment of time
    Time SystemTime()
    {
        return Time( (time( 0 ) * TICKS_PER_SEC) + 
            (ProgramTime().getTicks() % TICKS_PER_SEC) );
    }

    // returns current moment of time
    Time SteadyTime()
    {
        return Time( Time::start_ticks + ProgramTime() );
    }

// ------------------------------- Timer ------------------------------------

    // protected static constant for the reset of Stop time
    const Duration Timer::PrestartT = Duration(-1);

    // default constructor. Does NOT start the timer
    Timer::Timer()
    {
        StartT = StopT = LastT = ProgramTime();
    }

    // Start function. Timer starts calculating time if has not been calculating
    // before. Adds argument dt to allready calculated time
    void Timer::Start(const Duration& dt)
    {
        StartT = LastT = ProgramTime();
        StopT = PrestartT;
        CountedT += dt;
    }

    // Timer stops calculating time. If has been calculating before call - updates
    // calculated time before stop. Returns calculated time
    Duration Timer::Stop()
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
    Duration Timer::Reset(const Duration& dt)
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
    Duration Timer::Restart(const Duration& dt)
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
    Duration Timer::Update()
    {
        if (StopT < StartT)
        {
            TempT = ProgramTime();
            CountedT += TempT - LastT;
            LastT = TempT;
        }
        return CountedT;
    }

}
