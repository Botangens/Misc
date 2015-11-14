#pragma once

#include <time.h>
#include <stdint.h>

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

const time_int64_t MAX_CLOCK = ( (time_int64_t) 1 << ( sizeof( clock_t ) * 8 - 1 ) ) - 1;

const time_real_t SEC_PER_CLOCK = 1. / CLOCKS_PER_SEC;

#if CLOCKS_PER_SEC >= 1000000
const time_real_t USEC_PER_CLOCK = 1000000. / CLOCKS_PER_SEC;
#define CLOCKS_PER_USEC ( CLOCKS_PER_SEC / 1000000 )
#endif
#if CLOCKS_PER_SEC >= 1000
const time_real_t MSEC_PER_CLOCK = 1000. / CLOCKS_PER_SEC;
#define CLOCKS_PER_MSEC ( CLOCKS_PER_SEC / 1000 )
#endif



class Time;

class DeltaTime
{
  friend class Time;
  friend DeltaTime operator*(const time_real_t& v, const DeltaTime& dt);
  friend DeltaTime seconds(const time_real_t& v);
  friend DeltaTime millisec(const clock_t& v);
  friend DeltaTime microsec(const clock_t& v);
  friend DeltaTime ProgramTime();
protected:
  clock_t ticks;

  //clock_t tosex() { return ticks / CLOCKS_PER_SEC; }

public:
  DeltaTime(): ticks(0) {}
  DeltaTime(const clock_t& v): ticks(v) {}
  DeltaTime(const DeltaTime& dt): ticks(dt.ticks) {}

  inline DeltaTime& operator= (const DeltaTime& dt)
  {
    ticks = dt.ticks;
    return *this;
  }

  inline DeltaTime& operator+= (const DeltaTime& dt)
  {
    ticks += dt.ticks;
    return *this;
  }
  inline DeltaTime& operator-= (const DeltaTime& dt)
  {
    ticks -= dt.ticks;
    return *this;
  }
  inline DeltaTime& operator*= (const time_real_t& v)
  {
    ticks *= v;
    return *this;
  }
  inline DeltaTime& operator/= (const time_real_t& v)
  {
    ticks /= v;
    return *this;
  }

  inline DeltaTime operator+ (const DeltaTime& dt)  { return DeltaTime( ticks + dt.ticks ); }
  inline DeltaTime operator- (const DeltaTime& dt)  { return DeltaTime( ticks - dt.ticks ); }
  inline DeltaTime operator* (const time_real_t& v)  { return DeltaTime( clock_t( time_real_t(ticks) * v ) ); }
  inline DeltaTime operator/ (const time_real_t& v)  { return DeltaTime( clock_t( time_real_t(ticks) / v ) ); }
  inline Time operator+ (const Time& T);

  inline bool operator== (const DeltaTime& dt)  { return ticks == dt.ticks; }
  inline bool operator!= (const DeltaTime& dt)  { return ticks != dt.ticks; }
  inline bool operator> (const DeltaTime& dt)  { return ticks > dt.ticks; }
  inline bool operator< (const DeltaTime& dt)  { return ticks < dt.ticks; }
  inline bool operator>= (const DeltaTime& dt)  { return ticks >= dt.ticks; }
  inline bool operator<= (const DeltaTime& dt)  { return ticks <= dt.ticks; }

  inline const clock_t& clocks() const { return ticks; }

  inline time_real_t operator/ (const DeltaTime& dt)  { return (time_real_t)ticks / (time_real_t)dt.ticks; }

  inline operator time_real_t ()  { return time_real_t( (time_real_t)ticks / CLOCKS_PER_SEC ); }

#if CLOCKS_PER_SEC >= 1000000
  inline time_real_t asMicroSecF() { return (time_real_t)ticks * USEC_PER_CLOCK; }
  inline time_real_t asMicroSec() { return (time_real_t)ticks / CLOCKS_PER_USEC; }
#endif
#if CLOCKS_PER_SEC >= 1000
  inline time_real_t asMilliSecF() { return (time_real_t)ticks * MSEC_PER_CLOCK; }
  inline time_real_t asMilliSec() { return (time_real_t)ticks / CLOCKS_PER_MSEC; }
#endif
  inline time_real_t asSecF() { return (time_real_t)ticks * SEC_PER_CLOCK; }
  inline time_real_t asSec() { return (time_real_t)ticks / CLOCKS_PER_SEC; }

};

inline DeltaTime operator*(const time_real_t& v, const DeltaTime& dt)
{
	return DeltaTime(clock_t(time_real_t(dt.ticks) * v));
}

#if CLOCKS_PER_SEC >= 1000000
inline DeltaTime millisec(const clock_t& v)  { return DeltaTime(clock_t(v * CLOCKS_PER_USEC)); }
#endif
#if CLOCKS_PER_SEC >= 1000
inline DeltaTime microsec(const clock_t& v)  { return DeltaTime(clock_t(v * CLOCKS_PER_MSEC)); }
#endif
inline DeltaTime seconds(const time_real_t& v)  { return DeltaTime(clock_t(v * CLOCKS_PER_SEC)); }

inline DeltaTime ProgramTime()
{
    return DeltaTime( clock() );
}

class Time
{
  friend class DeltaTime;
  friend Time SystemTime();
protected:
  time_int64_t ticks;
public:
  static time_t sex;
  const static time_int64_t start_ticks;
public:
  Time(): ticks(0) {}
  Time(const time_int64_t& t): ticks(t) {}
  Time(const Time& T): ticks(T.ticks) {}

  inline Time& operator= (const Time& T)
  {
    ticks = T.ticks;
    return *this;
  }

  inline Time& operator+= (const DeltaTime& dt)
  {
    ticks += dt.ticks;
    return *this;
  }
  inline Time& operator-= (const DeltaTime& dt)
  {
    ticks -= dt.ticks;
    return *this;
  }

  inline Time operator+ (const DeltaTime& dt)  { return Time( ticks + dt.ticks ); }
  inline Time operator- (const DeltaTime& dt)  { return Time( ticks - dt.ticks ); }
  inline DeltaTime operator- (const Time& T)  { return DeltaTime( clock_t( ticks - T.ticks ) ); }

  inline bool operator== (const Time& T)  { return ticks == T.ticks; }
  inline bool operator!= (const Time& T)  { return ticks != T.ticks; }
  inline bool operator>= (const Time& T)  { return ticks >= T.ticks; }
  inline bool operator<= (const Time& T)  { return ticks <= T.ticks; }
  inline bool operator> (const Time& T)  { return ticks > T.ticks; }
  inline bool operator< (const Time& T)  { return ticks < T.ticks; }

  inline operator time_real_t () { return time_real_t((time_real_t)ticks / CLOCKS_PER_SEC); }

  inline const time_int64_t& times() const { return ticks; }
  inline time_t as_time_t() const { return time_t( ticks / CLOCKS_PER_SEC ); }

  tm as_tm()
  {
    sex = time_t( ticks / CLOCKS_PER_SEC );
    return *localtime( &sex );
  }
  char* as_c_str()
  {
    sex = time_t( ticks / CLOCKS_PER_SEC );
    return ctime( &sex );
  }

};

inline Time DeltaTime::operator+ (const Time& T) { return Time( (time_int64_t)ticks + T.ticks ); }

inline Time SystemTime()
{ 
    return Time( Time::start_ticks + clock() );
}


/////////////////////////////////////////////////////////////////////////////

class Timer
{
public:
	Timer();
	~Timer();
};
/*
const time_t timer = time(NULL);
printf("%s\n", ctime(&timer));


time_t t = time(NULL);
tm* aTm = localtime(&t);
printf("%04d/%02d/%02d %02d:%02d:%02d \n", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
getchar();
*/
