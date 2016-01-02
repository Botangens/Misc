/*
* FixedPoint.hpp - header file for fixed point library. Provides special
* integer-based type for calculations of real numbers with fixed point -
* steady precision.
*/

#pragma once

#include <stdexcept>
#include <cstdint>

namespace FixedPointNumerics
{
#define FPN FixedPointNumber

// ========================== Class FixedPointNumber ========================

    // This is Fixed-Point Numbers for fast arithmetics. Fractional part has
    // fixed size, given by template argument (10 digits by default), thus all
    // computations are being performed in integer types. This allow fast
    // operations and more accurate memory usage IF you could estimate
    // calculations` properties at precompile time.
    template<size_t N = 10,                 // amount of fractional digits
            class MainInt = int32_t,        // main storage type
            class DoubleInt = int64_t,      // temporal double precision type for calculations
            MainInt VALUE_AMPLITUDE = 0,    // max possible value to convert from (auto defined if 0)
            MainInt CREMENT_VALUE = 0>      // unit value for ++|-- operations (auto defined if 0)
    struct FPN
    {
    private:
        template<size_t NN, class MI, class DI, MI V, MI C>
        friend FPN<NN, MI, DI, V, C> sqrt( const FPN<NN, MI, DI, V, C>& );
    public:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Static members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        typedef MainInt MInt;  // main storage type
        typedef DoubleInt DInt;  // temporal double precision type for calculations

        constexpr static MInt BIT_SHIFT{ N };  // amount of fractional digits
        constexpr static MInt UNIT{ (1 << BIT_SHIFT) };  // single outer unit in inner units
    protected:
        constexpr static float FLOAT_UNIT{ (1 << BIT_SHIFT) };  // float value matching single unit
        constexpr static float FLOAT_UNIT_R{ 1. / (1 << BIT_SHIFT) };  // float value matching reversed single unit
        constexpr static double DOUBLE_UNIT{ (1 << BIT_SHIFT) };  // double value matching single unit
        constexpr static double DOUBLE_UNIT_R{ 1. / (1 << BIT_SHIFT) };  // double value matching reversed single unit

        constexpr static DInt SQRT_A{ 1 << (BIT_SHIFT >> 1) };  // for fast sqrt approximations
        constexpr static DInt SQRT_B{ 0xB504 << (BIT_SHIFT >> 1) };  // for fast sqrt approximation
        constexpr static DInt SQRT_EPSILON{ BIT_SHIFT == 10 ? 14 : 1 };   //  14 is optimal for 10bit shift

        constexpr static MInt _CREMENT = CREMENT_VALUE ? CREMENT_VALUE : UNIT;  // private value for ++|--
    public:
        constexpr static MInt MIN_VAL = VALUE_AMPLITUDE ?
            (-VALUE_AMPLITUDE - 1) : ((MInt)1 << (sizeof( MInt ) * 8 - 1));  // minimal possible stored value
        constexpr static MInt MAX_VAL = -1 - MIN_VAL;  // maximal possible stored value

        constexpr static MInt MAX_INT{ MAX_VAL >> BIT_SHIFT };  // max possible represented integer
        constexpr static MInt MIN_INT{ MIN_VAL >> BIT_SHIFT };  // min possible represented integer
        constexpr static float MAX_FLOAT{ MAX_VAL / FLOAT_UNIT };  // max possible represented float
        constexpr static float MIN_FLOAT{ MIN_VAL / FLOAT_UNIT };  // min possible represented float
        constexpr static double MAX_DOUBLE{ MAX_VAL / DOUBLE_UNIT };  // max possible represented double
        constexpr static double MIN_DOUBLE{ MIN_VAL / DOUBLE_UNIT };  // min possible represented double

    protected:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        MInt v;  // MAIN stored value

// -------------------------- Constructors/destroctor -----------------------
        FPN( const MInt& V, const bool& ) : v( V ) {}  // private support constructor
    public:
        FPN() : v( 0 ) {}
        FPN( const int& i ) { v = i << BIT_SHIFT; }  // unsafe convert int to FPN
        FPN( const float& f ) { v = (MInt)(f * FLOAT_UNIT); }   // unsafe convert float to FPN
        FPN( const double& d ) { v = (MInt)(d * FLOAT_UNIT); }   // unsafe convert double to FPN
        FPN( const FPN& F ) : v( F.v ) {}  // simple copy
        ~FPN() = default;

// -------------------------------- Comparisons -----------------------------
        inline bool operator == ( const FPN& f ) const { return v == f.v; }
        inline bool operator > ( const FPN& f ) const { return v > f.v; }
        inline bool operator < ( const FPN& f ) const { return v < f.v; }
        inline bool operator >= ( const FPN& f ) const { return v >= f.v; }
        inline bool operator <= ( const FPN& f ) const { return v <= f.v; }

// -------------------------------- Arithmetics -----------------------------
        inline FPN operator + ( const FPN& f ) const { return FPN( v + f.v, true ); }
        inline FPN operator - ( const FPN& f ) const { return FPN( v - f.v, true ); }
        inline FPN operator * ( const FPN& f ) const
        {
            return FPN( MInt( ((DInt)v * (DInt)f.v) >> BIT_SHIFT ), true );
        }
        inline FPN operator / ( const FPN& f ) const  //  division by zero is considered to be division by positive infinitesimal value
        {
            return (f.v == 0) ?
                FPN( v << BIT_SHIFT, true ) :
                FPN( MInt( ((DInt)v << BIT_SHIFT) / (DInt)f.v ), true );
        }

// -------------------------------- Assignments -----------------------------
        inline FPN& operator = ( const FPN& f )
        {
            v = f.v;
            return *this;
        }
        inline FPN& operator += ( const FPN& f )
        {
            v += f.v;
            return *this;
        }
        inline FPN& operator -= ( const FPN& f )
        {
            v -= f.v;
            return *this;
        }
        inline FPN& operator *= ( const FPN& f )
        {
            v = MInt( ((DInt)v * (DInt)f.v) >> BIT_SHIFT );
            return *this;
        }
        inline FPN& operator /= ( const FPN& f )    //  <<-- division by zero is considered to be division by positive infinitesimal value
        {
            (f.v == 0) ? v <<= BIT_SHIFT : v = MInt( ((DInt)v << BIT_SHIFT) / (DInt)f.v );
            return *this;
        }

// ---------------------------- Increment/Decrement -------------------------
        inline FPN& operator ++()
        {
            v += _CREMENT;
            return *this;
        }
        inline FPN operator ++(int)
        {
            FPN T( *this );
            v += _CREMENT;
            return T;
        }
        inline FPN& operator --()
        {
            v -= _CREMENT;
            return *this;
        }
        inline FPN operator --(int)
        {
            FPN T( *this );
            v -= _CREMENT;
            return T;
        }

// ------------------------------- Miscellaneous ----------------------------
        FPN& fromInt( const int& i )  // safe but slower conversion from int. Throw invalid_argument if parameter could not be represented
        {
            if( i > MAX_INT || i < MIN_INT )
                throw std::invalid_argument( "value out of representable range" );
            v = i << BIT_SHIFT;
            return *this;
        }
        FPN& fromFloat( const float& f )  // safe but slower conversion from float. Throw invalid_argument if parameter could not be represented
        {
            if( f > MAX_FLOAT || f < MIN_FLOAT)
                throw std::invalid_argument( "value out of representable range" );
            v = (MInt)(f * FLOAT_UNIT);
            return *this;
        }
        FPN& fromDouble( const double& d )  // safe but slower conversion from double. Throw invalid_argument if parameter could not be represented
        {
            if( d > MAX_DOUBLE || d < MIN_DOUBLE )
                throw std::invalid_argument( "value out of representable range" );
            v = (MInt)(d * FLOAT_UNIT);
            return *this;
        }

        inline operator int () const { return v >> BIT_SHIFT; }  // convert to int
        inline operator float () const { return (float)v * FLOAT_UNIT_R; }  // convert to float
        inline operator double () const { return (double)v * DOUBLE_UNIT_R; }  // convert to double

        inline int toInt() const { return v >> BIT_SHIFT; }  // convert to int
        inline float toFloat() const { return (float)v * FLOAT_UNIT_R; }  // convert to float
        inline double toDouble() const { return (double)v * DOUBLE_UNIT_R; }  // convert to double

        inline MInt& StoredValue() { return v; }  // should NOT be used in general

    };

// ~~~~~~~~~~~~~~~~~~~~~~~~ Exterior functionality ~~~~~~~~~~~~~~~~~~~~~~~~~~

    // sqrt function based on FPN calculations. Generally should be faster
    // than conversions to|from double and calculation of cmath -> sqrt, but
    // no warranty. Return FPN = '-1' if illegal argument.
    template<size_t N, class MI, class DI, MI V, MI C>
    FPN<N, MI, DI, V, C> sqrt( const FPN<N, MI, DI, V, C>& fpn )  // TOTALY HAND-MADE! Be careful - may cause bugs
    {
        if( fpn.v <= 0 )  return FPN<N, MI, DI, V, C> ( -!!fpn.v );  // TODO: reconsider 'throw'

        DI A{ (DI)fpn.v << FPN<N, MI, DI, V, C>::BIT_SHIFT };
        DI x0{ A / FPN<N, MI, DI, V, C>::SQRT_B + FPN<N, MI, DI, V, C>::SQRT_A }, x1;   //  [1482910, 32] optimal in 10bit case

        while( abs( (x1 = x0) - (x0 = (x0 + A/x0) >> 1) ) > FPN<N, MI, DI, V, C>::SQRT_EPSILON );

        return FPN<N, MI, DI, V, C>( (MI)x0, true );
    }

}
// ==========================================================================
