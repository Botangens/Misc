/*
* Perspective module for speed-up development of advanced time functionality.
* Depends on Perspective::Timer.hpp
*/

#pragma once

// ------------------------- Project dependencies ---------------------------
#include "Timer.hpp"

const Perspective::time_tick_t _TPS = Perspective::TICKS_PER_SEC;

namespace Perspective
{
// ========================== External functions ============================

    // make current thread inactive for a given duration. Has OS-dependent 
    // precision. Based on std::thread, so have to be std::thread-compatible.
    // Should not consume any CPU power.
    void Sleep(const Duration& Dur);

    // make current thread inactive until a given time (or duration since 
    // program start). Has OS-dependent precision. Based on std::thread, so 
    // have to be std::thread-compatible. Should not consume any CPU power.
    void SleepUntil(const Duration& Dur);
    void SleepUntil(const Time& Tm);

// ============================ Utility classes =============================

// ---------------------------- Elementary Timer ----------------------------

    // the most simple timer based on ProgramTime(). Has no virtual members. 
    // Has size equal to single Duration instance. Made for extreamly fast 
    // time requests.
    class ElementaryTimer
    {
    protected:
        Duration StartT = ProgramTime();  // simply start time - time interval since program start
    public:
        inline const Duration& GetTime() const { return ProgramTime() - StartT; }  // returns time since timer has started. Has inner subtraction operation
        inline const Duration& GetStart() const { return StartT; }  // simply returns current start time
        inline void Reset() { StartT = ProgramTime(); }  // resets timer start time to current program time
    };

// ------------------------------- Expectant --------------------------------

    // simple template class. Provides functionality for awaiting for either 
    // a duration or a point of time calculated by specific timer.
    template<class TimerType = Timer>
    class Expectant
    {
    protected:
        TimerType* PTimer;  // observed timer pointer
        Duration Expected;  // expected time point (stored as a duration since prog start) 
    public:
        Expectant( TimerType* PT ) : PTimer( PT ) {}  // simple and only constructor for setting the timer pointer

        inline void waitFor( const Duration& Dur ) { Expected = Dur + ProgramTime(); }  // begin awaiting for a duration since now
        inline void waitUntil( const Duration& Dur ) { Expected = Dur; }  // begin awaiting for a specific time point
        inline bool check() const { return PTimer->GetTime() >= Expected ? true : false; }  // check if the time has come
    };

// ------------------------------- Repeater ---------------------------------

    // simple template class. Provides functionality for repeatedly performed
    // time checks based on a specific timer.
    template<class TimerType = Timer>
    class Repeater
    {
    protected:
        TimerType* PTimer;  // observed timer pointer
        Duration Period;  // current repeate period
        Duration Expected{ ZERO_Duration };  // expected time point (stored as a duration since prog start)
    public:
        // simple constructor for setting Timer pointer and optianally repeat period
        Repeater( TimerType* PT, const Duration& Per = seconds( 1 ) ) : PTimer( PT ), Period( Per ) {}

        inline void repeat( const Duration& Per ) { Expected = ProgramTime() + (Period = Per); }  // call for repeating with new period
        inline void repeat() { Expected += Period; }  // call for repeating with same period
        inline bool check() const { return PTimer->GetTime() >= Expected ? true : false; }  // check if the time has come
    };

}
