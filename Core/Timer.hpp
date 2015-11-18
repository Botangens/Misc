/*
 * Perspective module for time management
 */

#pragma once

// External dependencies: <ctime>, <cstdint>. Both OS dependent.
#include <time.h>
#include <stdint.h>

// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------

// OS dependent constants for controlling clocks value range
const time_int64_t MAX_CLOCK = ( (time_int64_t)1 << ( sizeof( clock_t ) * 8 - 1 ) ) - 1;
const time_int64_t MIN_CLOCK = - ( (time_int64_t)1 << ( sizeof( clock_t ) * 8 - 1 ) );

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

// ============================ DeltaTime ===================================

// global time class predeclaration
class Time;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Class DeltaTime - main class for handling time events. Represents time
// interval. Provides: safe time algebra, conversions to/from real,
// collaboration with global time.
class DeltaTime
{
  // friends for safety
  friend class Time;
  friend DeltaTime operator*(const time_real_t& v, const DeltaTime& dt);
  friend DeltaTime seconds(const time_real_t& v);
  friend DeltaTime millisec(const clock_t& v);
  friend DeltaTime microsec(const clock_t& v);
  friend DeltaTime ProgramTime();
protected:
  clock_t ticks;  // <- Only data is a single 'clock_t' variable

public:
  // -------------------------- Methods --------------------------------------

  DeltaTime(): ticks(0) {}  // default constructor, creates zero-length time interval.
  DeltaTime(const clock_t& v): ticks(v) {}  // main constructor. May work with clock();
  DeltaTime(const DeltaTime& dt): ticks(dt.ticks) {}  // copy constructor

  inline DeltaTime& operator= (const DeltaTime& dt)  // assignment operator
  {
    ticks = dt.ticks;
    return *this;
  }

  // ------------------------ Time algebra ------------------------------------

  inline DeltaTime& operator+= (const DeltaTime& dt)  // addition assignment
  {
    ticks += dt.ticks;
    return *this;
  }
  inline DeltaTime& operator-= (const DeltaTime& dt)  // subtraction assignment
  {
    ticks -= dt.ticks;
    return *this;
  }
  inline DeltaTime& operator*= (const time_real_t& v)  // real scaling multiplication assignment
  {
    ticks *= v;
    return *this;
  }
  inline DeltaTime& operator/= (const time_real_t& v)  // real scaling division assignment
  {
    ticks /= v;
    return *this;
  }

  inline DeltaTime operator+ (const DeltaTime& dt)  { return DeltaTime( ticks + dt.ticks ); }  // addition
  inline DeltaTime operator- (const DeltaTime& dt)  { return DeltaTime( ticks - dt.ticks ); }  // subtraction
  inline DeltaTime operator* (const time_real_t& v)  { return DeltaTime( clock_t( time_real_t(ticks) * v ) ); }  // scaling multiplication
  inline DeltaTime operator/ (const time_real_t& v)  { return DeltaTime( clock_t( time_real_t(ticks) / v ) ); }  // scaling division
  inline Time operator+ (const Time& T);  // global time addition - reversed order operands

  // --------------------------- Comparisons ----------------------------------

  inline bool operator== (const DeltaTime& dt)  { return ticks == dt.ticks; }
  inline bool operator!= (const DeltaTime& dt)  { return ticks != dt.ticks; }
  inline bool operator> (const DeltaTime& dt)  { return ticks > dt.ticks; }
  inline bool operator< (const DeltaTime& dt)  { return ticks < dt.ticks; }
  inline bool operator>= (const DeltaTime& dt)  { return ticks >= dt.ticks; }
  inline bool operator<= (const DeltaTime& dt)  { return ticks <= dt.ticks; }

  // ------------------------ Various functionality ---------------------------

  inline const clock_t& clocks() const { return ticks; }  // protected data access

  // time-time division: returns real scaling factor
  inline time_real_t operator/ (const DeltaTime& dt)  { return (time_real_t)ticks / (time_real_t)dt.ticks; }

  // real type conversion: returns time interval in seconds
  inline operator time_real_t ()  { return time_real_t( (time_real_t)ticks / CLOCKS_PER_SEC ); }

  // OS dependent functionality: provides max-accurate possible conversions
#if CLOCKS_PER_SEC >= 1000000
  inline time_real_t asMicroSecF() { return (time_real_t)ticks * USEC_PER_CLOCK; }  // !DANGER! fast but may cause rounding errors
  inline time_real_t asMicroSec() { return (time_real_t)ticks / CLOCKS_PER_USEC; }  // contains real-type division
#endif
#if CLOCKS_PER_SEC >= 1000
  inline time_real_t asMilliSecF() { return (time_real_t)ticks * MSEC_PER_CLOCK; }  // !DANGER! fast but may cause rounding errors
  inline time_real_t asMilliSec() { return (time_real_t)ticks / CLOCKS_PER_MSEC; }  // contains real-type division
#endif
  inline time_real_t asSecF() { return (time_real_t)ticks * SEC_PER_CLOCK; }  // !DANGER! fast but may cause rounding errors
  inline time_real_t asSec() { return (time_real_t)ticks / CLOCKS_PER_SEC; }  // contains real-type division

};

// ~~~~~~~~~~~~~~~~~~~~~~~~ External functionality ~~~~~~~~~~~~~~~~~~~~~~~~~~

// constant static DeltaTime instances
const static DeltaTime ZERO_DeltaTime = DeltaTime();  // zero-time interval
const static DeltaTime SMALLEST_DeltaTime = DeltaTime( 1 );  // smallest possible time interval
const static DeltaTime MAX_DeltaTime = DeltaTime( MAX_CLOCK );  // maximal deltatime value
const static DeltaTime MIN_DeltaTime = DeltaTime( MIN_CLOCK );  // minimal deltatime value

inline DeltaTime operator*(const time_real_t& v, const DeltaTime& dt)  // reversed order operands real scaling multiplication
{
	return DeltaTime(clock_t(time_real_t(dt.ticks) * v));
}

// OS dependent external constructors of various precision
#if CLOCKS_PER_SEC >= 1000000
inline DeltaTime millisec(const clock_t& v)  { return DeltaTime(clock_t(v * CLOCKS_PER_USEC)); }
#endif
#if CLOCKS_PER_SEC >= 1000
inline DeltaTime microsec(const clock_t& v)  { return DeltaTime(clock_t(v * CLOCKS_PER_MSEC)); }
#endif
inline DeltaTime seconds(const time_real_t& v)  { return DeltaTime(clock_t(v * CLOCKS_PER_SEC)); }

// !IMPORTANT! main function for retrieving current time interval. Returns time
// elapsed since program has started.
inline DeltaTime ProgramTime()  // returns time interval since program has started
{
    return DeltaTime( clock() );
}

// ============================== Time =====================================

// Class Time. Represents moment of time, contains ticks since epoch. Can be
// converted to c-struct tm and/or string timestamp, etc. Has same precision as
// DeltaTime but generally covers greater time interval (as contains date-year
// info). Nevertheless absolute time value may have 1 second numerical error
// (coz of time() function). As the Time represents moment of time - the
// algebra is passed to DeltaTime. Only one existing Time-Time operation is
// subtraction, which returns difference between two moments of time, which is
// obviously a deltatime.
class Time
{
  // friends for safety
  friend class DeltaTime;
  friend Time SystemTime();
protected:
  time_int64_t ticks;  // <- main data - amount of ticks since epoch

  static time_t sex;  // totally protected. Temporal variable for conversion to seconds
  const static time_int64_t start_ticks;  // totally protected. Program start Time (1 sec error possible)
public:
  // -------------------------- Methods --------------------------------------

  Time(): ticks( Time::start_ticks ) {}  // default constructor. Returns program start moment (1 sec error possible). Synonym to StartMoment
  Time(const time_int64_t& t): ticks(t) {}  // main constructor. Generally should not be called
  Time(const Time& T): ticks(T.ticks) {}  // copy constructor

  inline Time& operator= (const Time& T)  // assignment operator
  {
    ticks = T.ticks;
    return *this;
  }

  inline Time& operator+= (const DeltaTime& dt)  // addition assignment
  {
    ticks += dt.ticks;
    return *this;
  }
  inline Time& operator-= (const DeltaTime& dt)  // subtraction of interval assignment
  {
    ticks -= dt.ticks;
    return *this;
  }

  inline Time operator+ (const DeltaTime& dt)  { return Time( ticks + dt.ticks ); }  // addition
  inline Time operator- (const DeltaTime& dt)  { return Time( ticks - dt.ticks ); }  // subtraction of interval
  inline DeltaTime operator- (const Time& T)  { return DeltaTime( clock_t( ticks - T.ticks ) ); }  // subtraction of moment

  // --------------------------- Comparisons ----------------------------------

  inline bool operator== (const Time& T)  { return ticks == T.ticks; }
  inline bool operator!= (const Time& T)  { return ticks != T.ticks; }
  inline bool operator>= (const Time& T)  { return ticks >= T.ticks; }
  inline bool operator<= (const Time& T)  { return ticks <= T.ticks; }
  inline bool operator> (const Time& T)  { return ticks > T.ticks; }
  inline bool operator< (const Time& T)  { return ticks < T.ticks; }

  // ------------------------ Various functionality ---------------------------

  // real-type conversion: returns amount of seconds since epoch.
  inline operator time_real_t () { return time_real_t((time_real_t)ticks / CLOCKS_PER_SEC); }

  inline const time_int64_t& times() const { return ticks; }  // protected data access

  inline time_t as_time_t() const { return time_t( ticks / CLOCKS_PER_SEC ); } // C-style time_t conversion

  tm as_tm()  // C-style struct tm conversion
  {
    sex = time_t( ticks / CLOCKS_PER_SEC );
    return *localtime( &sex );
  }

  char* as_c_str()  // C-style string of char date-stamp conversion
  {
    sex = time_t( ticks / CLOCKS_PER_SEC );
    return ctime( &sex );
  }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~ External functionality ~~~~~~~~~~~~~~~~~~~~~~~~~~

inline Time DeltaTime::operator+ (const Time& T) { return Time( (time_int64_t)ticks + T.ticks ); }  // reversed order operands interval addition

// !IMPORTANT! main function. Returns current moment of time. Returned value
// contains approximate amount of ticks since epoch. Although Time is as
// accurate as DeltaTime - it depends from time() function, which has
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
// functionality. Contains 5 DeltaTimes (4 accessible) and several methods
// for different time management approaches. Time should be an abstract
// algebraic concept. Therefore timer operates with the amounts of time
// (DeltaTimes), but not the moments of time. Time moments are also stored as
// DeltaTimes for memory efficiency and speed.
// Basically works as simple Start-Stop-Get timer.
class Timer
{
protected:
    DeltaTime StartT{ ZERO_DeltaTime };  // a moment of Start event
    DeltaTime StopT{ ZERO_DeltaTime };  // a moment of Stop event or PrestartT if timer IsOn (IsOn <=> Stop < Start)
    DeltaTime CountedT{ ZERO_DeltaTime };  // amount of counted time
    DeltaTime LastT{ ZERO_DeltaTime };  // a moment of last Update event

    DeltaTime TempT{ ZERO_DeltaTime };  // totally protected. Temporal variable
    const static DeltaTime PrestartT;  // a moment 1 tick before program has started
public:
	Timer();  // simple default constructor
    virtual ~Timer() {}  // virtual destructor

    virtual void Start(const DeltaTime& dt = ZERO_DeltaTime);  // begins time calculation. Adds argument to calculated time
    virtual DeltaTime Stop();  // if WasOn - makes last time update. Stops time calculation, returns calculated time 
    virtual DeltaTime Reset(const DeltaTime& dt = ZERO_DeltaTime);  // if WasOn - makes last time update. Resets all members. Adds dt to calc time. Returns previously calculated time
    virtual DeltaTime Restart(const DeltaTime& dt = ZERO_DeltaTime);  // if WasOn - makes last time update. Restarts calculating time. Resets calc time with dt. Returns previously calculated time
    virtual DeltaTime Update();  // updates calculated time and returns it, but only if timer IsOn

    inline const DeltaTime& GetTime() const { return CountedT; }  // simple protected getter
    inline const DeltaTime& GetLast() const { return LastT; }  // simple protected getter
    inline const DeltaTime& GetStart() const { return StartT; }  // simple protected getter
    inline const DeltaTime& GetStop() const { return StopT; }  // simple protected getter
};
