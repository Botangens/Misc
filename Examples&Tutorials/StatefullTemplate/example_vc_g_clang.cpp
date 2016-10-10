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

*/

#include <iostream>
using namespace std;

// ==================== constexpr state flag processing =====================

//                              simplest case

// Function 'flag' works as a bool flag. If it could be evaluated without
// errors - we will consider it as 'true'. Else - it is 'false'.
// It is wrapped into struct and has respective argument is order to force
// Argument Dependant Lookup mechanism. Otherwise - behaviour is not stable.
// Integral return type is a must for constexpr. 
struct A { friend constexpr int flag(A); }; // A must be arg

// Instantiation of this template will generate an inline definition for
// function 'flag', thus it become evaluable. The scope containing the
// definition is the scope, where instantiation is plased.
// The argument is also present for ADL mechanism.
template<class C>
struct Setter { friend constexpr int flag(C) { return 0; } };

// Instantiation should be executed from within template function. Otherwise
// flag will be instantly set to 'true'.
// 'sizeof' provides guarantied instantiation of struct template.
template<class C = A, int = sizeof( Setter<C> )>
constexpr int set() { return 0; }


// In order to check the flag SFINAE mechanism is being used. At the 
// beginning preprocessor will attempt to instantiate the templated function
// with argument of more suitable type - 'int'.  If the substitution of 
// 'flag( C() )' is correct - flag is set. If it fails - preprocessor will 
// instantiate template function with less suitable argument of type 'float',
//  and thus return false.

template<class C, int = flag( C() )>
constexpr bool _get(int) { return true; }

template<class C>
constexpr bool _get(float) { return false; }

// Simple function that provides proper call for cleaner code.
template<class C = A, int R = _get<C>(0) >
constexpr int get() { return R; }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ value storing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Generally we could use the return value of a function.
struct VA{ friend constexpr int value(VA); };

// We could set the return value with template parameter.
template<int V, class C>
struct VSetter { friend constexpr int value(C) { return V; } };

// Same kind of setting function.
template<int V, class C = VA, int = sizeof (VSetter<V, C>)>
constexpr int setV() { return 0; }


// Now instead of bare evaluation-possibility check we may actually call 
// the flag function and receive its result.

template<class C, int R = value( C() )>
constexpr int _getV(int) { return R; }

template<class C>
constexpr int _getV(float) { return -1; }

template<class C = VA, int R = _getV<C>(0)>
constexpr int getV() { return R; }

// ~~~~~~~~~~~~~~~~~~~~~~~~ simple constexpr array ~~~~~~~~~~~~~~~~~~~~~~~~~~

// Flag/value function could be easily overloaded by argument. This also works
// well with ADL.
template<int N>
struct NFlag{ friend constexpr int nvalue( NFlag<N> ); };

// Now setter will define indexed function with specified return value.
template<int N, int V>
struct NSetter { friend constexpr int nvalue( NFlag<N> ) { return V; } };

// As before - setter function just instantiates the template. In order to 
// add some bug resistance - flag function should also be declared before 
// defined. This is made by instantiating NFlag template.
template<int N, int V, int = sizeof(NFlag<N>), int R = sizeof( NSetter<N ,V> )>
constexpr int setNV( int = 0) { return R; }

// Value retrieving is almost the same as before. The only difference is 
// default value. One may simply generate errors for undefined values by
// removing (or commenting out) the 'float' argument overload.

template<int N, int R = nvalue( NFlag<N>() )>
constexpr int _getNV(int) { return R; }

template<int N>
constexpr int _getNV(float) { return -1; }

template<int N, int R = _getNV<N>(0) >
constexpr int getNV() { return R; }

// -------------------------- recursive setters -----------------------------

// One may provide mechanism for consecutive (recursive) flag/value settings.
// This may be implemented by overloading the setter function template.
// SFINAE rule will help to find last instantiated 'Setter'.  

// Function with 'float' argument will be evaluated (including template 
// parameter substitution) only if function evaluation of function with 'int' 
// argument causes substitution failure.
// If evaluated - this function instantiates next 'value' and returns index.
template<int V, int N>
constexpr int _setNextNV( float, int = sizeof( NSetter<N, V> ) ) { return N; }

// Evaluation of function with 'int' argument means that corresponding flag 
// has been set. In this case recursive call with the next index is being 
// performed.
template<int V, int N, int = nvalue( NFlag<N>() ) >
constexpr int _setNextNV( int ) { return _setNextNV<V, N + 1>( 0 ); }

// Simple 'start-of-searching' function for nicer look may be provided
template<int V, int N = 0, int R = _setNextNV<V, N>( 0 ) >
constexpr int setNextNV() { return R; }

// Naturally it is possible to implement recursive observer, that will only
// return last defined flag/value or it`s index without setting the next one.

// ======================== constexpr argument print ========================

// constexpr evaluation could be easily performed by passing the value as a
// template parameter rather than function argument.
template<int I>
void Print() { cout<< I <<endl; }

// ================================ main ====================================

int main ()
{  
    // one may see how the flag is being checked and set.
    Print<get()>();
    set();
    Print<get()>();

    cout << endl;

    // value storing also looks nice.
    Print<getV()>();
    setV<3>();
    Print<getV()>();

    cout << endl;

    // this is the code for the values with specific index.
    Print<getNV<0>()>();
    setNV<0,7>();
    Print<getNV<0>()>();    
    setNV<1,8>();
    Print<getNV<1>()>();

    cout << endl;

    // consecutive 'pushing' of values looks like this.
    setNextNV<91>();
    Print<getNV<2>()>();
    setNextNV<92>();
    Print<getNV<3>()>();
    setNextNV<-93>();
    Print<getNV<4>()>();

    cin.get();
}

// by Botangens https://github.com/Botangens