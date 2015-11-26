/*
 * Perspective module for time management.
 * Similar to std::chrono, but self-made accidently.
 * Depends on <ctime>, <cstdint>
 * Several types can be externally defined before first include
 */

#pragma once

// External dependencies: <ctime>, <cstdint>. Both OS dependent.
#include <time.h>
#include <stdint.h>

// ---------------------------------------------------------------------------

namespace Perspective
{

    // Inner types for data calculations. Can be defined externally.
#ifndef TIME_REAL_T
    typedef double time_real_t;
#else
    typedef TIME_REAL_T time_real_t;
#endif

#ifndef TIME_INT64_T
    typedef int64_t time_int64_t;
#else
    typedef TIME_INT64_T time_int64_t;
#endif

#ifndef TIME_INT_T
    typedef int time_int_t;
#else
    typedef TIME_INT_T time_int_t;
#endif

    // --------------------------------------------------------------------------

        // OS dependent constants for controlling clocks value range
    const time_int64_t MAX_CLOCK = ((time_int64_t)1 << (sizeof( clock_t ) * 8 - 1)) - 1;
    const time_int64_t MIN_CLOCK = -((time_int64_t)1 << (sizeof( clock_t ) * 8 - 1));

    // OS dependent constants for fast but less accurate calculations
    const time_real_t SEC_PER_CLOCK = 1. / CLOCKS_PER_SEC;

#if CLOCKS_PER_SEC >= 1000000
    const time_real_t USEC_PER_CLOCK = 1000000. / CLOCKS_PER_SEC;
#define CLOCKS_PER_USEC ( CLOCKS_PER_SEC / 1000000 )
#endif

#if CLOCKS_PER_SEC >= 1000
    const time_real_t MSEC_PER_CLOCK = 1000. / CLOCKS_PER_SEC;
#define CLOCKS_PER_MSEC ( CLOCKS_PER_SEC / 1000 )
#endif

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
        friend Duration millisec( const clock_t& v );
        friend Duration microsec( const clock_t& v );
        friend Duration ProgramTime();
    protected:
        clock_t ticks;  // <- Only data is a single 'clock_t' variable

    public:
        // -------------------------- Methods --------------------------------------

        Duration() : ticks( 0 ) {}  // default constructor, creates zero-length time interval.
        Duration( const clock_t& v ) : ticks( v ) {}  // main constructor. May work with clock();
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
        inline Duration operator* ( const time_real_t& v ) const { return Duration( clock_t( time_real_t( ticks ) * v ) ); }  // scaling multiplication
        inline Duration operator/ ( const time_real_t& v ) const { return Duration( clock_t( time_real_t( ticks ) / v ) ); }  // scaling division
        inline Time operator+ ( const Time& T );  // global time addition - reversed order operands

        // --------------------------- Comparisons ----------------------------------

        inline bool operator== ( const Duration& dt ) const { return ticks == dt.ticks; }
        inline bool operator!= ( const Duration& dt ) const { return ticks != dt.ticks; }
        inline bool operator> ( const Duration& dt ) const { return ticks > dt.ticks; }
        inline bool operator< ( const Duration& dt ) const { return ticks < dt.ticks; }
        inline bool operator>= ( const Duration& dt ) const { return ticks >= dt.ticks; }
        inline bool operator<= ( const Duration& dt ) const { return ticks <= dt.ticks; }

        // ------------------------ Various functionality ---------------------------

        inline const clock_t& clocks() const { return ticks; }  // protected data access

        // time-time division: returns real scaling factor
        inline time_real_t operator/ ( const Duration& dt ) const { return (time_real_t)ticks / (time_real_t)dt.ticks; }

        // time-time division ramainder
        inline Duration operator% ( const Duration& dt ) const { return Duration( ticks % dt.ticks ); }

        // real type conversion: returns time interval in seconds
        inline operator time_real_t () const { return time_real_t( (time_real_t)ticks / CLOCKS_PER_SEC ); }

        // OS dependent functionality: provides max-accurate possible conversions
    #if CLOCKS_PER_SEC >= 1000000
        inline time_real_t asMicroSec() const { return (time_real_t)ticks / CLOCKS_PER_USEC; }  // contains real-type division
        inline time_real_t asMicroSecFast() const { return (time_real_t)ticks * USEC_PER_CLOCK; }  // !DANGER! fast but may cause rounding errors
        inline time_int_t asMicroSecInt() const { return (time_int_t)ticks / CLOCKS_PER_USEC; }  // contains int-type division
    #endif
    #if CLOCKS_PER_SEC >= 1000
        inline time_real_t asMilliSec() const { return (time_real_t)ticks / CLOCKS_PER_MSEC; }  // contains real-type division
        inline time_real_t asMilliSecFast() const { return (time_real_t)ticks * MSEC_PER_CLOCK; }  // !DANGER! fast but may cause rounding errors
        inline time_int_t asMilliSecInt() const { return (time_int_t)ticks / CLOCKS_PER_MSEC; }  // contains int-type division
    #endif
        inline time_real_t asSec() const { return (time_real_t)ticks / CLOCKS_PER_SEC; }  // contains real-type division
        inline time_real_t asSecFast() const { return (time_real_t)ticks * SEC_PER_CLOCK; }  // !DANGER! fast but may cause rounding errors
        inline time_int_t asSecInt() const { return (time_int_t)ticks / CLOCKS_PER_SEC; }  // contains int-type division

    };

    // ~~~~~~~~~~~~~~~~~~ Duration External functionality ~~~~~~~~~~~~~~~~~~~~~~~

        // constant static Duration instances
    const static Duration ZERO_Duration = Duration();  // zero-time interval
    const static Duration SMALLEST_Duration = Duration( 1 );  // smallest possible time interval
    const static Duration MAX_Duration = Duration( MAX_CLOCK );  // maximal deltatime value
    const static Duration MIN_Duration = Duration( MIN_CLOCK );  // minimal deltatime value

    inline Duration operator*( const time_real_t& v, const Duration& dt )  // reversed order operands real scaling multiplication
    {
        return Duration( clock_t( time_real_t( dt.ticks ) * v ) );
    }

    // OS dependent external constructors of various precision
#if CLOCKS_PER_SEC >= 1000000
    inline Duration microsec( const clock_t& v ) { return Duration( clock_t( v * CLOCKS_PER_USEC ) ); }
#endif
#if CLOCKS_PER_SEC >= 1000
    inline Duration millisec( const clock_t& v ) { return Duration( clock_t( v * CLOCKS_PER_MSEC ) ); }
#endif
    inline Duration seconds( const time_real_t& v ) { return Duration( clock_t( v * CLOCKS_PER_SEC ) ); }

    // !IMPORTANT! main function for retrieving current time interval. Returns time
    // elapsed since program has started.
    inline Duration ProgramTime()  // returns time interval since program has started
    {
        return Duration( clock() );
    }

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
        friend Time SystemTime();
    protected:
        time_int64_t ticks;  // <- main data - amount of ticks since epoch

        static time_t sex;  // totally protected. Temporal variable for conversion to seconds
        const static time_int64_t start_ticks;  // totally protected. Program start Time (1 sec error possible)
    public:
        // -------------------------- Methods --------------------------------------

        Time() : ticks( Time::start_ticks ) {}  // default constructor. Returns program start moment (1 sec error possible). Synonym to StartMoment
        Time( const time_int64_t& t ) : ticks( t ) {}  // main constructor. Generally should not be called
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
        inline operator time_real_t () const { return time_real_t( (time_real_t)ticks / CLOCKS_PER_SEC ); }

        inline const time_int64_t& times() const { return ticks; }  // protected data access

        inline time_t as_time_t() const { return time_t( ticks / CLOCKS_PER_SEC ); } // C-style time_t conversion

        tm as_tm() const  // C-style struct tm conversion
        {
            sex = time_t( ticks / CLOCKS_PER_SEC );
            return *localtime( &sex );
        }

        char* as_c_str() const  // C-style string of char date-stamp conversion
        {
            sex = time_t( ticks / CLOCKS_PER_SEC );
            return ctime( &sex );
        }
    };

    // ~~~~~~~~~~~~~~~~~~~~~ Time External functionality ~~~~~~~~~~~~~~~~~~~~~~~

    inline Time Duration::operator+ ( const Time& T ) { return Time( (time_int64_t)ticks + T.ticks ); }  // reversed order operands interval addition

    // !IMPORTANT! main function. Returns current moment of time. Returned value
    // contains approximate amount of ticks since epoch. Although Time is as
    // accurate as Duration - it depends from time() function, which has
    // 1 second precision. Therefore it constantly contains up to 1 second
    // difference with global time.
    inline Time SystemTime()  // returns current moment of time
    {
        return Time( Time::start_ticks + clock() );
    }

    // Moment of time when the program has started. 1 second error may
    // occur (read 'SystemTime' description)
    static const Time StartMoment = Time();

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
        const static Duration PrestartT;  // a moment 1 tick before program has started
    public:
        Timer();  // simple default constructor
        virtual ~Timer() {}  // virtual destructor

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