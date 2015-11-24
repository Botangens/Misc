/* TimeUtils Realizations
 * Depends on Perspective::Timer.hpp, <chrono>, <thread>
 */

#include "TimeUtils.hpp"

// External dependencies: <chrono>, <thread>
#include <chrono>  // std::chrono::duration
#include <thread>  // std::this_tread::sleep_for

// --------------------------------------------------------------------------

namespace Perspective
{

    void Sleep(const Duration& Dur)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( Dur.asMilliSecInt() ) );
    }

    void SleepUntil(const Duration& Dur)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds( (Dur - ProgramTime()).asMilliSecInt() )
            );
    }

    void SleepUntil(const Time& Tm)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds( (Tm - SystemTime()).asMilliSecInt() )
            );
    }

}
