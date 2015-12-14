/* TimeUtils Realizations
 * Depends on Perspective::Timer.hpp, <chrono>, <thread>
 */

#include "TimeUtils.hpp"

// Standart dependencies: <thread>
#include <thread>  // std::this_tread::sleep_for

// ----------------------- Local utility functions --------------------------

inline void _Sleep( const Perspective::Duration& Dur )
{

// seems like this attempt does not work
//#if _TPS >= 1000000
//    std::cout<<"--1000000"<<std::endl;
//    std::this_thread::sleep_for( std::chrono::microseconds( Dur.asMicroSecInt() ) );
//#elif _TPS >= 1000
//    std::cout<<"--1000"<<std::endl;
//    std::this_thread::sleep_for( std::chrono::milliseconds( Dur.asMilliSecInt() ) );
//#else
//    //std::cout<<"--1"<<std::endl;
//    std::this_thread::sleep_for( std::chrono::seconds( Dur.asSecInt() ) );
//#endif

    // simple and nice
    std::this_thread::sleep_for( std::chrono::milliseconds( Dur.asMilliSecInt() ) );

}

// --------------------------------------------------------------------------

namespace Perspective
{

    void Sleep( const Duration& Dur ) { _Sleep( Dur ); }

    void SleepUntil( const Duration& Dur ) { _Sleep ( Dur - ProgramTime() ); }

    void SleepUntil( const Time& Tm ) { _Sleep ( Tm - SystemTime() ); }

}
