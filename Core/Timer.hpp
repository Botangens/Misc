/*
* Perspective module for time management.
* Similar to std::chrono, but self-made accidently.
* Depends on <ctime>, <cstdint>
* .cpp depends on <chrono>
* Several types can be externally defined before first include
*/

#pragma once

// Standert dependencies: <ctime>, <cstdint>. Both OS dependent.
#include <time.h>
#include <stdint.h>
#include <chrono>

#ifdef PER_TIME_WINDOWS_QPC
#include <Windows.h>
#endif

// ============================== Type declarations =========================

namespace Perspective
{

// --------------------------------------------------------------------------

#ifdef PER_TIME_WINDOWS_QPC  // Windows query performance counter implementation

#ifndef TIME_INT64_T
    typedef __int64 time_int64_t;
#else
    typedef TIME_INT64_T time_int64_t;
#endif

    // main type to store ticks
    typedef __int64 time_tick_t;

    // amount of calculated ticks in one second
    extern const time_tick_t TICKS_PER_SEC;  // defined in .cpp

#else  // std::chrono-based manual implementation (default)
    
#ifndef PER_TIME_CHRONO_HYBRID
#define PER_TIME_CHRONO_HYBRID
#endif

#ifndef TIME_INT64_T
    typedef int64_t time_int64_t;
#else
    typedef TIME_INT64_T time_int64_t;
#endif

    // main type to store ticks
    typedef std::chrono::steady_clock::rep time_tick_t;

    // amount of calculated ticks in one second
    static const time_tick_t TICKS_PER_SEC =
        std::chrono::steady_clock::period::den /
        std::chrono::steady_clock::period::num;

#endif

// ---------------------------------------------------------------------------

    // Inner types for data calculations. Can be defined externally.
#ifndef TIME_REAL_T
    typedef double time_real_t;
#else
    typedef TIME_REAL_T time_real_t;
#endif

#ifndef TIME_INT_T
    typedef int time_int_t;
#else
    typedef TIME_INT_T time_int_t;
#endif

// ~~~~~~~~~~~~~~~~~~~~ Implementation-dependant constants ~~~~~~~~~~~~~~~~~~

    // implementation-dependent constants for controlling ticks value range
    constexpr time_tick_t MIN_TICK = (time_tick_t)1 << (sizeof( time_tick_t ) * 8 - 1);
    constexpr time_tick_t MAX_TICK = -1 - MIN_TICK;
    //const time_tick_t MAX_TICK = ((time_tick_t)1 << (sizeof( time_tick_t ) * 8 - 1)) - 1;
    //const time_tick_t MIN_TICK = MAX_TICK + 1; //-((time_tick_t)1 << (sizeof( time_tick_t ) * 8 - 1));

    // Private constant for transforming ticks into CPU time clocks
    static const clock_t TICKS_PER_CLOCK = TICKS_PER_SEC / CLOCKS_PER_SEC;

    // OS dependent constants for fast but less accurate calculations
    static const time_real_t SEC_PER_TICK = 1. / TICKS_PER_SEC;

    static const time_real_t USEC_PER_TICK = 1000000. / TICKS_PER_SEC;
#define TICKS_PER_USEC ( TICKS_PER_SEC / 1000000 )

    static const time_real_t MSEC_PER_TICK = 1000. / TICKS_PER_SEC;
#define TICKS_PER_MSEC ( TICKS_PER_SEC / 1000 )

// ============================ Duration ===================================

    // global time class predeclaration
    class Time;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Class Duration - main class for handling time events. Represents time
    // interval. Provides: safe time algebra, conversions to/from real,
    // collaboration with global time.
    class Duration
    {
        // friends for safety
        friend class Time;
        friend Duration operator*( const time_real_t& v, const Duration& dt );
        friend Duration seconds( const time_real_t& v );
        friend Duration millisec( const time_tick_t& v );
        friend Duration microsec( const time_tick_t& v );
        friend Duration ProgramTime();
    protected:
        time_tick_t ticks;  // the class contains a single number

    public:

// -------------------------- Methods --------------------------------------

        Duration() : ticks( 0 ) {}  // default constructor, creates zero-length time interval.
        Duration( const time_tick_t& v ) : ticks( v ) {}  // main constructor. May work with clock();
        Duration( const Duration& dt ) : ticks( dt.ticks ) {}  // copy constructor

        inline Duration& operator= ( const Duration& dt )  // assignment operator
        {
            ticks = dt.ticks;
            return *this;
        }

// ------------------------ Time algebra ------------------------------------

        inline Duration& operator+= ( const Duration& dt )  // addition assignment
        {
            ticks += dt.ticks;
            return *this;
        }
        inline Duration& operator-= ( const Duration& dt )  // subtraction assignment
        {
            ticks -= dt.ticks;
            return *this;
        }
        inline Duration& operator*= ( const time_real_t& v )  // real scaling multiplication assignment
        {
            ticks *= v;
            return *this;
        }
        inline Duration& operator/= ( const time_real_t& v )  // real scaling division assignment
        {
            ticks /= v;
            return *this;
        }

        inline Duration operator+ ( const Duration& dt ) const { return Duration( ticks + dt.ticks ); }  // addition
        inline Duration operator- ( const Duration& dt ) const { return Duration( ticks - dt.ticks ); }  // subtraction
        inline Duration operator* ( const time_real_t& v ) const { return Duration( time_tick_t( time_real_t( ticks ) * v ) ); }  // scaling multiplication
        inline Duration operator/ ( const time_real_t& v ) const { return Duration( time_tick_t( time_real_t( ticks ) / v ) ); }  // scaling division
        inline Time operator+ ( const Time& T );  // global time addition - reversed order operands

// --------------------------- Comparisons ----------------------------------

        inline bool operator== ( const Duration& dt ) const { return ticks == dt.ticks; }
        inline bool operator!= ( const Duration& dt ) const { return ticks != dt.ticks; }
        inline bool operator> ( const Duration& dt ) const { return ticks > dt.ticks; }
        inline bool operator< ( const Duration& dt ) const { return ticks < dt.ticks; }
        inline bool operator>= ( const Duration& dt ) const { return ticks >= dt.ticks; }
        inline bool operator<= ( const Duration& dt ) const { return ticks <= dt.ticks; }

// ------------------------ Various functionality ---------------------------

        inline const time_tick_t& getTicks() const { return ticks; }  // protected data exposition

        // time-time division: returns real scaling factor
        inline time_real_t operator/ ( const Duration& dt ) const { return (time_real_t)ticks / (time_real_t)dt.ticks; }

        // time-time division ramainder
        inline Duration operator% ( const Duration& dt ) const { return Duration( ticks % dt.ticks ); }

        // real type conversion: returns time interval in seconds
        inline operator time_real_t () const { return time_real_t( (time_real_t)ticks / TICKS_PER_SEC ); }

        inline time_real_t asMicroSec() const { return (time_real_t)ticks / TICKS_PER_USEC; }  // contains real-type division
        inline time_real_t asMicroSecFast() const { return (time_real_t)ticks * USEC_PER_TICK; }  // !DANGER! fast but may cause rounding errors
        inline time_int_t asMicroSecInt() const { return (time_int_t)(ticks / TICKS_PER_USEC); }  // contains int-type division

        inline time_real_t asMilliSec() const { return (time_real_t)ticks / TICKS_PER_MSEC; }  // contains real-type division
        inline time_real_t asMilliSecFast() const { return (time_real_t)ticks * MSEC_PER_TICK; }  // !DANGER! fast but may cause rounding errors
        inline time_int_t asMilliSecInt() const { return (time_int_t)(ticks / TICKS_PER_MSEC); }  // contains int-type division

        inline time_real_t asSec() const { return (time_real_t)ticks / TICKS_PER_SEC; }  // contains real-type division
        inline time_real_t asSecFast() const { return (time_real_t)ticks * SEC_PER_TICK; }  // !DANGER! fast but may cause rounding errors
        inline time_int_t asSecInt() const { return (time_int_t)(ticks / TICKS_PER_SEC); }  // contains int-type division

    };

// ~~~~~~~~~~~~~~~~~~ Duration External functionality ~~~~~~~~~~~~~~~~~~~~~~~

    // constant static Duration instances
    const static Duration ZERO_Duration = Duration();  // zero-time interval
    const static Duration SMALLEST_Duration = Duration( 1 );  // smallest possible time interval
    const static Duration MAX_Duration = Duration( MAX_TICK );  // maximal deltatime value
    const static Duration MIN_Duration = Duration( MIN_TICK );  // minimal deltatime value

    inline Duration microsec( const time_tick_t& v ) { return Duration( time_tick_t( v * TICKS_PER_USEC ) ); }
    inline Duration millisec( const time_tick_t& v ) { return Duration( time_tick_t( v * TICKS_PER_MSEC ) ); }
    inline Duration seconds( const time_real_t& v ) { return Duration( time_tick_t( v * TICKS_PER_SEC ) ); }

    inline Duration operator*( const time_real_t& v, const Duration& dt )  // reversed order operands real scaling multiplication
    {
        return Duration( time_tick_t( time_real_t( dt.ticks ) * v ) );
    }

    // !IMPORTANT! main function for retrieving current time interval. Returns time
    // interval since program has started.
    /* inline? */ inline Duration ProgramTime();  // returns time interval since program has started

    // Returns total approximate amount of CPU time consumed by all threads of
    // program since it has started
    /* inline? */ inline Duration ConsumedTime();  // returns amount of concumed CPU time

// ============================== Time =====================================

    // Class Time. Represents moment of time, contains ticks since epoch. Can be
    // converted to c-struct tm and/or string timestamp, etc. Has same precision as
    // Duration but generally covers greater time interval (as contains date-year
    // info). Nevertheless absolute time value may have 1 second numerical error
    // (coz of time() function). As the Time represents moment of time - the
    // algebra is passed to Duration. Only one existing Time-Time operation is
    // subtraction, which returns difference between two moments of time, which is
    // obviously a deltatime.
    class Time
    {
        // friends for safety
        friend class Duration;
        friend Duration ProgramTime();
        friend Duration ConsumedTime();
        friend Time SystemTime();
        friend Time GlobalTime();
    protected:
        time_tick_t ticks;  // <- main data - amount of ticks since epoch

        static time_t sex;  // totally protected. Temporal variable for conversion to seconds
        const static time_tick_t start_ticks;  // totally protected. Program start Time (1 sec error possible)
    public:

// -------------------------- Methods --------------------------------------

        Time() : ticks( Time::start_ticks ) {}  // default constructor. Returns program start moment (1 sec error possible). Synonym to StartMoment
        Time( const time_tick_t& t ) : ticks( t ) {}  // main constructor
        Time( const Time& T ) : ticks( T.ticks ) {}  // copy constructor

        inline Time& operator= ( const Time& T )  // assignment operator
        {
            ticks = T.ticks;
            return *this;
        }

        inline Time& operator+= ( const Duration& dt )  // addition assignment
        {
            ticks += dt.ticks;
            return *this;
        }
        inline Time& operator-= ( const Duration& dt )  // subtraction of interval assignment
        {
            ticks -= dt.ticks;
            return *this;
        }

        inline Time operator+ ( const Duration& dt ) const { return Time( ticks + dt.ticks ); }  // addition
        inline Time operator- ( const Duration& dt ) const { return Time( ticks - dt.ticks ); }  // subtraction of interval
        inline Duration operator- ( const Time& T ) const { return Duration( clock_t( ticks - T.ticks ) ); }  // subtraction of moment

// --------------------------- Comparisons ----------------------------------

        inline bool operator== ( const Time& T ) const { return ticks == T.ticks; }
        inline bool operator!= ( const Time& T ) const { return ticks != T.ticks; }
        inline bool operator>= ( const Time& T ) const { return ticks >= T.ticks; }
        inline bool operator<= ( const Time& T ) const { return ticks <= T.ticks; }
        inline bool operator> ( const Time& T ) const { return ticks > T.ticks; }
        inline bool operator< ( const Time& T ) const { return ticks < T.ticks; }

// ------------------------ Various functionality ---------------------------

        // real-type conversion: returns amount of seconds since epoch.
        inline operator time_real_t () const { return time_real_t( (time_real_t)ticks / TICKS_PER_SEC ); }

        inline const time_tick_t& getTicks() const { return ticks; }  // protected data exposition

        inline time_t as_time_t() const { return time_t( ticks / TICKS_PER_SEC ); } // C-style time_t conversion

        tm as_tm() const  // C-style struct tm conversion
        {
            sex = time_t( ticks / TICKS_PER_SEC );
            return *localtime( &sex );
        }

        char* as_c_str() const  // C-style string of char date-stamp conversion
        {
            sex = time_t( ticks / TICKS_PER_SEC );
            return ctime( &sex );
        }
    };

// ~~~~~~~~~~~~~~~~~~~~~ Time External functionality ~~~~~~~~~~~~~~~~~~~~~~~

    inline Time Duration::operator+ ( const Time& T ) { return Time( ticks + T.ticks ); }  // reversed order operands interval addition

    // !IMPORTANT! main function. Returns current moment of time. Returned 
    // value contains approximate amount of ticks since epoch. Although Time
    // is as accurate as Duration - it may depend from time() function, which
    // has 1 second precision. Therefore it constantly contains up to 1 second
    // difference with global time. Also SystemTime() is steady: it is not 
    // affected by system clock adjustments (like winter-summer time change)
    /* inline? */ inline Time SystemTime();  // returns current system time (steady)

    // Returns current moment of time in global time
    /* inline? */ inline Time GlobalTime();  // returns current time moment in global time

    // Moment of time when the program has started. 1 second error may
    // occur (read 'SystemTime' description)
    static const Time StartMoment = GlobalTime();

// ============================== Timer =====================================

    // Class Timer. Virtual class that provides basic time calculation
    // functionality. Contains 5 Durations (4 accessible) and several methods
    // for different time management approaches. Time should be an abstract
    // algebraic concept. Therefore timer operates with the amounts of time
    // (Durations), but not the moments of time. Time moments are also stored
    // as Durations for memory efficiency and speed.
    // Basically works as simple Start-Stop-Get timer.
    class Timer
    {
    protected:
        Duration StartT{ ZERO_Duration };  // a moment of Start event
        Duration StopT{ ZERO_Duration };  // a moment of Stop event or PrestartT if timer IsOn (IsOn <=> Stop < Start)
        Duration CountedT{ ZERO_Duration };  // amount of counted time
        Duration LastT{ ZERO_Duration };  // a moment of last Update event

        Duration TempT{ ZERO_Duration };  // totally protected. Temporal variable
        const static Duration PrestartT;  // a moment 1 tick before epoch
    public:
        Timer();  // simple default constructor
        virtual ~Timer() = default;  // virtual destructor

        virtual void Start( const Duration& dt = ZERO_Duration );  // begins time calculation. Adds argument to calculated time
        virtual Duration Stop();  // if WasOn - makes last time update. Stops time calculation, returns calculated time 
        virtual Duration Reset( const Duration& dt = ZERO_Duration );  // if WasOn - makes last time update. Resets all members. Adds dt to calc time. Returns previously calculated time
        virtual Duration Restart( const Duration& dt = ZERO_Duration );  // if WasOn - makes last time update. Restarts calculating time. Resets calc time with dt. Returns previously calculated time
        virtual Duration Update();  // updates calculated time and returns it, but only if timer IsOn

        inline const Duration& GetTime() const { return CountedT; }  // simple protected getter
        inline const Duration& GetLast() const { return LastT; }  // simple protected getter
        inline const Duration& GetStart() const { return StartT; }  // simple protected getter
        inline const Duration& GetStop() const { return StopT; }  // simple protected getter
    };
}

// ==========================================================================

// ============================ Implementations =============================

// ==========================================================================

namespace Perspective
{

#ifdef PER_TIME_WINDOWS_QPC

    struct _win_QPC_local
    {
        __int64 _tempTicks;  // temporal variable for ticks per second
        __int64 _startTicks;  // local variable for program time evaluation
        __int64 _startTimeTicks;  // local variable for ticks of start time.
        __int64 _startShifted;  // 
        __int64 _startTime;  //

        _win_QPC_local()
        {
            if (!QueryPerformanceFrequency( (LARGE_INTEGER*)&_tempTicks ))
                throw ("Performance counters not supported");
            QueryPerformanceCounter( (LARGE_INTEGER*)&_startTicks );
            _startTime = time( 0 );
            _startTimeTicks = _startTime * _tempTicks;
            _startShifted = _startTicks - _startTicks % _tempTicks;
        }
    } _QPC_local;  // temporal instance for initializations

    // period initialization
    const time_tick_t TICKS_PER_SEC = _QPC_local._tempTicks;

// ------------------------------ Duration ---------------------------------

    // implementation of main function. Actually must be inlined but I doubt.
    Duration ProgramTime()
    {
        QueryPerformanceCounter( (LARGE_INTEGER*)&_QPC_local._tempTicks );
        return Duration( _QPC_local._tempTicks - _QPC_local._startTicks );
    }

    // returns total approxymate amount of consumed by all trhreads CPU time
    Duration ConsumedTime()
    {
        return Duration( (time_tick_t)clock() * TICKS_PER_CLOCK );
    }

// -------------------------------- Time ------------------------------------

    // returns current moment of system time
    Time SystemTime()
    {
        QueryPerformanceCounter( (LARGE_INTEGER*)&_QPC_local._tempTicks );
        return Time( _QPC_local._startTimeTicks + _QPC_local._tempTicks - _QPC_local._startTicks );
    }

    // TODO: add system-not-steady implementation
    // IMPROVE: windows-QPC and time.h based global time getter is needed!
    // returns current moment of global time
    Time GlobalTime()
    {
        QueryPerformanceCounter( (LARGE_INTEGER*)&_QPC_local._tempTicks );
        return Time( _QPC_local._startTimeTicks + _QPC_local._tempTicks - _QPC_local._startTicks );
    }



#else

// ------------------------------ Duration ---------------------------------

    // implementation of main function. Actually must be inlined but I doubt.
    Duration ProgramTime()
    {
        return Duration( std::chrono::steady_clock::now().time_since_epoch().count() - Time::start_ticks );
    }

    // returns total approxymate amount of consumed by all trhreads CPU time
    Duration ConsumedTime()
    {
        return Duration( (time_tick_t)clock() * TICKS_PER_CLOCK );
    }

// -------------------------------- Time ------------------------------------

    // returns current moment of system time
    Time SystemTime()
    {
        return Time( std::chrono::steady_clock::now().time_since_epoch().count() );
    }

    // TODO: add system-not-steady implementation
    // returns current moment of global time
    Time GlobalTime()
    {
        return Time( std::chrono::system_clock::now().time_since_epoch().count() );
    }

#endif
}
