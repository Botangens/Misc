/*
 *  This example demonstrates simple mechanism for unification and inlining
 *  of various functions and function objects. 
 *
 *  Tesed with MSVS on windows and g++ on linux.
 *
 *  Require C++11.
 *
 *  Better use disassembly in debug mode.
 */

#include <iostream>
using namespace std;

// ==========================================================================

// base class - for virtuality mechanism
struct MyObj
{
    using FType = int( *)(int, int); // call semantics alias

    virtual int operator() ( int a, int b )
    {
        cout << "No default action!" << endl;
        return 0;
    }
    virtual ~MyObj() = default;
}defaultObj; // static default object


// generic wrapping class - the question is: what could be inlined?
template<class Func>
class Wrapping : public MyObj
{
    Func _f;
public:

    Wrapping( Func f ) : _f( f ) {};
    int operator()( int a, int b ) override { return _f( a, b ); }
};
// function wrapping class - specially for functions
template<class Func, Func f>
struct FWrapping : public MyObj
{
    int operator ()( int a, int b ) override { return f( a, b ); }
};

// wrapping maker functions
template<class Func>
Wrapping<Func>* Wrap( Func f ) // for objects
{
    static Wrapping<Func> W( f );
    return &W;
}
template<class Func, Func f>
FWrapping<Func, f>* Wrap() // for undefined semantics
{
    static FWrapping<Func, f> W;
    return &W;
}
template<MyObj::FType f>
FWrapping<MyObj::FType, f>* Wrap() // for the same semantics
{
    return Wrap<MyObj::FType, f>();
}

// shortcut 'clear type declarator'
#define declarate( X ) std::decay< decltype( X ) >::type

// --------------------------------------------------------------------------

//various functions for test

int subInl( int a, int b ) { return a - b; }  // same semantics inline function

float fsub( float a, float b ) { return a - b; }  // different semantics inlinefunction

const int odd( const int& a, int& b, double t ) { return a + b - t; }   // odd semantics 
                                                                        // inline function

int main()
{
    // various functors for tests:

    struct : public MyObj   // manually inherited structure (to compare with)
    {
        int operator()( int a, int b ) override { return a + b; };
    }addObj;

    int sub( int a, int b );    // simple function

    struct  // anonimus executable struct
    {
        int operator()( int a, int b ) { return a + b; }
    }add;
    
    struct XorStruct    // executable structure constructor
    {
        int operator()( int a, int b ) { return a ^ b; }
    };

    const auto& mul = []( int a, int b )->int {return a * b; }; // captured lambda
    
    // and later - noncaptured lambda

    MyObj* po{ &defaultObj };  // The pointer being set!

    // util variables
    int i, a, b, res;
    cout << "Enter i, a, b..." << endl;
    cin >> i >> a >> b;

    // just for short and clear switch
#define CASE( X, ... ) \
    case X: \
        po = __VA_ARGS__; \
        break;

    // selecting functor for proper tests. Input-dependant to avoid inlining of 'po'.
    switch( i )
    {
        CASE( 0, &addObj ); // manual inheritance

        CASE( 11, Wrap( sub ) );    // functor-like function wrap. Should not be inlined
        CASE( 12, Wrap<sub>() );    // same semantics wrap. Not always inlined by g++.
        CASE( 13, Wrap<decltype(sub)*, sub>() );    // different semantics wrap examples.
        CASE( 14, Wrap<declarate(sub), sub>() );    // Have the same effect.
        CASE( 15, Wrap( subInl ) ); // inlined function wraps. Should be always inlined
        CASE( 16, Wrap<subInl>() ); // in opposition to simple function, that is sometimes
                                    // not inlined by g++.
    
        CASE( 21, Wrap( fsub ) );   // functor-like wrap of different-type function
        //CASE( 22, Wrap<fsub>() ); // illegal because of types missmatch
        CASE( 23, Wrap<declarate( fsub ), fsub>() );    // different-type function wrap

        //CASE( 31, Wrap( odd ) );  // illegal because of totally different semantics
        //CASE( 32, Wrap<declarate( odd ), odd>() );    // yet illegal
        CASE( 33, Wrap( []( int a, int b )->int { return odd( a, b, 10.5 ); } ) ); // this
                // is OK. Odd function is being successfully called from within lambda
                // that could be inlined in its turn.

        CASE( 4, Wrap( []( int a, int b ) {return a / b; } ) ); // simply lambda wrap
        
        CASE( 5, Wrap( mul ) ); // captured lambda wrap

        CASE( 6, Wrap( add ) ); // captured functor wrap

        CASE( 7, Wrap( XorStruct() ) ); // wrap of functor captured by constructor call

        // one may add tests for wrapping objects created with 'new' and so on...
    }

    res = (*po)(a, b);  // watch this in disassemble
    cout << res << endl;

    res = (*po)(b, a);  // repeated call to avoid inlining of 'res'
    cout << res << endl;

    cout << "========= end ========" << endl;
    cin.get();
    cin.get();
    return 0;
}

int sub( int a, int b ) { return a - b; } // sub definition
