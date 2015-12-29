/*
* FixedPoint.hpp - header file for fixed point library. Provides special
* integer-based type for calculations of real numbers with fixed point -
* steady precision.
*/

#pragma once 

#include <exception> 
#include <cstdint> 

namespace FixedPointNumerics
{
#define FPN FixedPointNumber 

    // This is Fixed-Point Numbers for fast arithmetics. Fractional part is 
    // by default 10 bits (1/1024)  [defined by 'BIT_SHIFT' and 'FLOAT_UNIT'] 
    // and is actually given by template parameter 
    template<size_t N = 10, 
            class MainInt = int32_t, 
            class DoubleInt = int64_t, 
            MainInt VALUE_AMPLITUDE = 0,
            MainInt CREMENT_VALUE = 0>
    struct FPN
    {
    private:
        template<size_t NN, class MI, class DI, MI V, MI C> friend FPN<NN, MI, DI, V, C> sqrt( const FPN<NN, MI, DI, V, C>& );
    public:
        constexpr static MainInt BIT_SHIFT{ N };
        constexpr static float FLOAT_UNIT{ (1 << BIT_SHIFT) };
        constexpr static MainInt UNIT{ (1 << BIT_SHIFT) };

        constexpr static DoubleInt SQRT_A{ 1 << (BIT_SHIFT >> 1) };
        constexpr static DoubleInt SQRT_B{ 0xB504 << (BIT_SHIFT >> 1) };
        constexpr static DoubleInt SQRT_EPSILON{ BIT_SHIFT == 10 ? 14 : 1 };   //  14 is optimal for 10bit shift 

        constexpr static MainInt MIN_VAL = VALUE_AMPLITUDE ?
            (-VALUE_AMPLITUDE - 1) : ((MainInt)1 << (sizeof( MainInt ) * 8 - 1));
        constexpr static MainInt MAX_VAL = -1 - MIN_VAL;

        constexpr static MainInt MAX_INT{ MAX_VAL >> BIT_SHIFT };
        constexpr static MainInt MIN_INT{ MIN_VAL >> BIT_SHIFT };
        constexpr static float MAX_FLOAT{ MAX_VAL / FLOAT_UNIT };
        constexpr static float MIN_FLOAT{ MIN_VAL / FLOAT_UNIT };

    protected:
        constexpr static MainInt _CREMENT = CREMENT_VALUE ? CREMENT_VALUE : UNIT;

        MainInt v;    //    <<-- the value itself is 4-byte integer

        FPN( const MainInt& V, const bool& ) : v( V ) {}   //  <<-- simply copies the value 
    public:
        FPN() : v( 0 ) {}
        FPN( const int& i ) { v = i << BIT_SHIFT; }    //  <<-- unsafely converts int to FPN 
        FPN( const float& f ) { v = (MainInt)(f * FLOAT_UNIT); }    //  <<-- unsafely converts float to FPN 
        FPN( const double& d ) { v = (MainInt)(d * FLOAT_UNIT); }    //  <<-- unsafely converts double to FPN 
        FPN( const FPN& F ) : v( F.v ) {}

        inline bool operator== ( const FPN& f ) const { return v == f.v; }
        inline bool operator> ( const FPN& f ) const { return v > f.v; }
        inline bool operator< ( const FPN& f ) const { return v < f.v; }
        inline bool operator>= ( const FPN& f ) const { return v >= f.v; }
        inline bool operator<= ( const FPN& f ) const { return v <= f.v; }

        inline FPN operator+ ( const FPN& f ) const { return FPN( v + f.v, true ); }
        inline FPN operator- ( const FPN& f ) const { return FPN( v - f.v, true ); }
        inline FPN operator* ( const FPN& f ) const
        {
            return FPN( MainInt( ((DoubleInt)v * (DoubleInt)f.v) >> BIT_SHIFT ), true ); 
        }
        inline FPN operator/ ( const FPN& f ) const
        {
            return (f.v == 0) ?
                FPN( v << BIT_SHIFT, true ) :  //  division by zero is considered to be division by positive infinitesimal value 
                FPN( MainInt( ((DoubleInt)v << BIT_SHIFT) / (DoubleInt)f.v ), true );
        }

        inline FPN& operator = ( const FPN& f )
        {
            v = f.v;
            return *this;
        }
        inline FPN& operator+= ( const FPN& f )
        {
            v += f.v;
            return *this;
        }
        inline FPN& operator-= ( const FPN& f )
        {
            v -= f.v;
            return *this;
        }
        inline FPN& operator*= ( const FPN& f )
        {
            v = MainInt( ((DoubleInt)v * (DoubleInt)f.v) >> BIT_SHIFT );
            return *this;
        }
        inline FPN& operator/= ( const FPN& f )    //  <<-- division by zero is considered to be division by positive infinitesimal value 
        {
            (f.v == 0) ? v <<= BIT_SHIFT : v = MainInt( ((DoubleInt)v << BIT_SHIFT) / (DoubleInt)f.v );
            return *this;
        }

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

        FPN& fromInt( const int& i )
        {
            if( i > MAX_INT || i < MIN_INT )
                throw std::exception( "value out of range" );
            v = V << BIT_SHIFT;
            return *this;
        }
        FPN& fromFloat( const float& f )
        {
            if( f > MAX_FLOAT || f < MIN_FLOAT)
                throw std::exception( "value out of range" );
            v = (MainInt)(f * FLOAT_UNIT);
            return *this;
        }
        FPN& fromDouble( const double& d )
        {
            if( d > MAX_FLOAT || d < MIN_FLOAT )
                throw std::exception( "value out of range" );
            v = (MainInt)(d * FLOAT_UNIT);
            return *this;
        }

        inline operator int () const { return v >> BIT_SHIFT; }    //  <<-- converts to Int 
        inline operator float () const { return (float)v / FLOAT_UNIT; }    //  <<-- converts to float !!DANGER!! can couse data lost 
        inline operator double () const { return (double)v / FLOAT_UNIT; }    //  <<-- converts to double 
        
        inline int toInt() const { return v >> BIT_SHIFT; }    //  <<-- converts to Int 
        inline float toFloat() const { return (float)v / FLOAT_UNIT; }    //  <<-- converts to float !!DANGER!! can couse data lost 
        inline double toDouble() const { return (double)v / FLOAT_UNIT; }    //  <<-- converts to double 

        inline MainInt& StoredValue() { return v; }

    };

    template<size_t N, class MI, class DI, MI V, MI C>
    FPN<N, MI, DI, V, C> sqrt( const FPN<N, MI, DI, V, C>& fpn )    //  <<--TOTALY HAND-MADE! Be carefull - may couse bugs 
    {
        if( fpn.v <= 0 )  return FPN<N, MI, DI, V, C> ( -!!fpn.v );  // TODO: reconsider 'throw' 

        DI A{ (DI)fpn.v << FPN<N, MI, DI, V, C>::BIT_SHIFT };
        DI x0{ A / FPN<N, MI, DI, V, C>::SQRT_B + FPN<N, MI, DI, V, C>::SQRT_A }, x1;   //  [1482910, 32] optimal in 10bit case 
        
        while( abs( (x1 = x0) - (x0 = (x0 + A/x0) >> 1) ) > FPN<N, MI, DI, V, C>::SQRT_EPSILON );
        
        return FPN<N, MI, DI, V, C>( (MI)x0, true );
    }

}