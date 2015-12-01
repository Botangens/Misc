/*
* Timer.hpp implementations
*/

#include "Timer.hpp"

namespace Perspective
{

// ~~~~~~~~~~~~~~~~~~~~~~ OS-independent instantiations ~~~~~~~~~~~~~~~~~~~~~

    // protected static technical variable for temporal conversions to integer seconds
    time_t Time::sex{ 0 };

    // protected static constant for storaging approximate program start time
    const time_tick_t Time::start_ticks = SystemTime().getTicks();

// =========================== Timer (OS independant) =======================

    // protected static constant for the reset of Stop time
    const Duration Timer::PrestartT = Duration( -1 );

    // default constructor. Does NOT start the timer
    Timer::Timer()
    {
        StartT = StopT = LastT = ProgramTime();
    }

    // Start function. Timer starts calculating time if has not been calculating
    // before. Adds argument dt to allready calculated time
    void Timer::Start( const Duration& dt )
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
    Duration Timer::Reset( const Duration& dt )
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
    Duration Timer::Restart( const Duration& dt )
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
