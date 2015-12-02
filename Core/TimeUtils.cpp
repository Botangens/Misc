/* TimeUtils Realizations
 * Depends on Perspective::Timer.hpp, <chrono>, <thread>
 */

#include "TimeUtils.hpp"

// External dependencies: <chrono>, <thread>
#include <chrono>  // std::chrono::duration
#include <thread>  // std::this_tread::sleep_for

// ----------------------- Local utility functions --------------------------

inline void _Sleep( const Perspective::Duration& Dur )
{
#if _TPS >= 1000000
    std::this_thread::sleep_for( std::chrono::microseconds( Dur.asMicroSecInt() ) );
#elif _TPS >= 1000
    std::this_thread::sleep_for( std::chrono::milliseconds( Dur.asMilliSecInt() ) );
#else
    std::this_thread::sleep_for( std::chrono::seconds( Dur.asSecInt() ) );
#endif

    // UNDONE: check that is above on windows and if does not work for ms
    // duration - replace all with next:
    //std::this_thread::sleep_for( std::chrono::milliseconds( Dur.asMilliSecInt() ) );
}

// --------------------------------------------------------------------------

namespace Perspective
{

    void Sleep( const Duration& Dur ) { _Sleep( Dur ); }

    void SleepUntil( const Duration& Dur ) { _Sleep ( Dur - ProgramTime() ); }

    void SleepUntil( const Time& Tm ) { _Sleep ( Tm - SystemTime() ); }

}
