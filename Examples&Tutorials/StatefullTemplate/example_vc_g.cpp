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

Clang compiler will definitely not compile this code because of several bugs
and different preprocessor behaviour while deducing templates. Some workaround
is provided in other file. 

The code should be used 'as it is' and only for educational purposes. Any
modifications or implementations are not guaranteed to be working.

*/

#include <iostream>
using namespace std;

// ==================== constexpr state flag processing =====================

//                              simplest case

// This function works as flag. If it could be evaluated without errors - 
// we will consider it as 'true'. Else - it is 'false'.
// Integral return type is a must for constexpr. 
constexpr int flag();

// Instantiation of this template will generate an inline definition for
// function 'flag', thus it become evaluable. The scope containing the
// definition is the scope, where instantiation is plased.
template<int>
struct Setter { friend constexpr int flag() { return 0; } };

// Instantiation should be executed from within template function in order to
// delay it. Otherwise flag will be instantly set to 'true'.
// 'sizeof' provides guarantied instantiation of struct template.
template<int N = 0, int = sizeof( Setter<N> )>
constexpr int set() { return 0; }

// Flag check may be performed as a simple noexcept. It will check if 'flag'
// could be evaluated, yet without evaluation itself. 
template<int R = noexcept(flag())>
constexpr int get() { return R; }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ value storing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Generally we could use the return value of a function.
constexpr int value();

// We could set the return value with template parameter.
template<int V = 0>
struct VSetter { friend constexpr int value() { return V; } };

// Same kind of setting function.
template<int V, int = sizeof(VSetter<V>)>
constexpr int setV() { return 0; }

// Now one may get the value in case it could be evaluated
template<int R = value()>  // cause compile error if not set
// or even provide some default value if needed.
//template<int R = noexcept(value()) ? value() : -1 > // '-1' - default value.
constexpr int getV() { return R; }

// ~~~~~~~~~~~~~~~~~~~~~~~~ simple constexpr array ~~~~~~~~~~~~~~~~~~~~~~~~~~

// Flag/value function could be easily overloaded by argument. Also 
// declaration and definition tricks may require Argument Dependent Lookup 
// for stable behavior on some compilers.
template<int N>
struct NFlag { friend constexpr int nvalue( NFlag<N> ); };

// Now setter will define indexed function with specified return value.
template<int N, int V>
struct NSetter { friend constexpr int nvalue( NFlag<N> ) { return V; } };

// As before - setter function just instantiates the template.
template<int N, int V, int = sizeof(NSetter<N, V>) >
constexpr int setNV() { return 0; }

// Value retrieving is almost the same as before:
//template<int N, int R = nvalue(NFlag<N>())>  // cause compile error if not set
template<int N, int R = noexcept(nvalue(NFlag<N>())) ? nvalue(NFlag<N>()) : -1 >
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
constexpr int _setNextNV( float, int = sizeof(NSetter<N, V>) ) { return N; }

// Evaluation of function with 'int' argument means that corresponding flag 
// has been set. In this case recursive call with the next index is being 
// performed.
template<int V, int N, int = nvalue( NFlag<N>() ) >
constexpr int _setNextNV( int ) { return _setNextNV<V, N + 1>(0); }

// Simple 'start-of-searching' function for nicer look may be provided
template<int V, int N = 0, int R = _setNextNV<V, N>(0) >
constexpr int setNextNV() { return R; }

// Naturally it is possible to implement recursive observer, that will only
// return last defined flag/value or it`s index without setting the next one.

// ======================== constexpr argument print ========================

// constexpr evaluation could be easily performed by passing the value as a
// template parameter rather than function argument.
template<int I>
void Print() { cout << I << endl; }

// ================================ main ====================================

int main ()
{
    // simple flag checks and setting
    Print<get()>();
    set();
    Print<get()>();

    cout << endl;

    // value setting and retrieving
    Print<getV()>();
    setV<3>();
    Print<getV()>();

    cout << endl;

    // specific value manipulations
    Print<getNV<0>()>();
    setNV<0,7>();
    Print<getNV<0>()>();    
    setNV<1,8>();
    Print<getNV<1>()>();

    cout << endl;

    // consecutive value manipulations
    setNextNV<91>();
    Print<getNV<2>()>();
    setNextNV<92>();
    Print<getNV<3>()>();
    setNextNV<-93>();
    Print<getNV<4>()>();
    
    cin.get();
}

// by Botangens https://github.com/Botangens