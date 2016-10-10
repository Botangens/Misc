/*
_____________________________________________________________________________

This file contains some example and simple expanation of statefull template
metaprogramming. It is highly sophisticated technique and thus require 
deep understanding of what is going on and what could happen with such
code inside preprocessor and compiler.

The reader should be aware of the following rules, those could be found 
for example http://en.cppreference.com/w/ :
- SFINAE (substitution failure is not an error)
- ADL (argument dependent lookup)
- 'constexpr' specifier semantics for functions
- 'sizeof' operator behavior
- 'noexcept' operator behavior (including the bug with constexpr in clang)
- 'friend' specifier behavior for functions and inline functions
- template class and template function rules:
    * explicit and implicit instantiation
    * template argument deduction
    * function template overloading
    * (IMPORTANT) default template parameter deduction and substitution
    
This implementation was inspired by the following article:
http://b.atch.se/posts/constexpr-counter/ 
_____________________________________________________________________________

The code has been tested with vc++ and g++ for c++11/c++14. 
All other compilers (versions) may give unstable result.

Due to some bugs in clang compiler this file contains some workaround. 
The code is much more complicated, yet it is more stable and better suits
c++ rules avoiding some 'implementation-defined' sections. 

The code should be used 'as it is' and only for educational purposes. Any
modifications or implementations are not guaranteed to be working.

The code was intentionally left without proper comments for cleaner look.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
!!!! More detailed explanations are given in similar file with comments. !!!!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

#include <iostream>
using namespace std;

// ==================== constexpr state flag processing =====================

//                              simplest case

struct A { friend constexpr int flag(A); }; // A must be arg

template<class C>
struct Setter { friend constexpr int flag(C) { return 0; } };

template<class C = A, int = sizeof (Setter<C>)>
constexpr int set() { return 0; }


template<class C, int = flag( C() )>
constexpr bool _get(int) { return true; }

template<class C>
constexpr bool _get(float) { return false; }

template<class C = A, int R = _get<C>(0) >
constexpr int get() { return R; }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ value storing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct VA{ friend constexpr int value(VA); };

template<int V, class C>
struct VSetter { friend constexpr int value(C) { return V; } };

template<int V, class C = VA, int = sizeof (VSetter<V, C>)>
constexpr int setV() { return 0; }


template<class C, int R = value( C() )>
constexpr int _getV(int) { return R; }

template<class C>
constexpr int _getV(float) { return -1; }

template<class C = VA, int R = _getV<C>(0)>
constexpr int getV() { return R; }

// ~~~~~~~~~~~~~~~~~~~~~~~~ simple constexpr array ~~~~~~~~~~~~~~~~~~~~~~~~~~

template<int N>
struct NFlag{ friend constexpr int nvalue( NFlag<N> ); };

template<int N, int V>
struct NSetter { friend constexpr int nvalue( NFlag<N> ) { return V; } };

template<int N, int V, int = sizeof(NFlag<N>), int R = sizeof( NSetter<N ,V> )>
constexpr int setNV( int = 0) { return R; }


template<int N, int R = nvalue( NFlag<N>() )>
constexpr int _getNV(int) { return R; }

template<int N>
constexpr int _getNV(float) { return -1; }

template<int N, int R = _getNV<N>(0) >
constexpr int getNV() { return R; }

// -------------------------- recursive setters -----------------------------

template<int V, int N>
constexpr int _setNextNV( float, int = sizeof( NSetter<N, V> ) ) { return N; }

template<int V, int N, int = nvalue( NFlag<N>() ) >
constexpr int _setNextNV( int ) { return _setNextNV<V, N + 1>( 0 ); }

template<int V, int N = 0, int R = _setNextNV<V, N>( 0 ) >
constexpr int setNextNV() { return R; }

// ======================== constexpr argument print ========================

template<int I>
void Print() { cout<< I <<endl; }

// ================================ main ====================================

int main ()
{  
    Print<get()>();
    set();
    Print<get()>();

    cout << endl;

    Print<getV()>();
    setV<3>();
    Print<getV()>();

    cout << endl;

    Print<getNV<0>()>();
    setNV<0,7>();
    Print<getNV<0>()>();    
    setNV<1,8>();
    Print<getNV<1>()>();

    cout << endl;

    setNextNV<91>();
    Print<getNV<2>()>();
    setNextNV<92>();
    Print<getNV<3>()>();
    setNextNV<-93>();
    Print<getNV<4>()>();

    cin.get();
}

// by Botangens https://github.com/Botangens