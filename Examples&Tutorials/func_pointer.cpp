/*

Simple example of function pointers, method pointers and functors.

*/

#include <iostream>
using namespace std;

// ---- the easy part - simple function pointer ----  
double get4() { return 4; }
double get6() { return 6; }

void print( double ( *foo )() )
{
    cout << foo() << endl;
}

// ---- the HARD PART! templated member functor in templated class ---- 

// templated 2d vector
template<class C>
class V
{
public:
    C x;
    C y;

    V( const C& X = 0, const C& Y = 0 ) : x( X ), y( Y ) {}

    C& getx() { return x; }
    C& gety() { return y; }

};

// template of member class-functor
template<class C>
class Member
{
    C v;
public:
    Member( const C& c ) : v( c ) {}

    operator C& () { return v; }
    C& operator () () { return v; }
};

// flexible output
template< class C, class FOO = C& (V<C>::*)()>
void print( V<C>& v, FOO foo )
{
    cout << (v.*foo)() << endl;
}

// --------------- main ----------------

int main()
{
    print( get4 );
    print( get6 );

    cout << "--------\n";

    typedef V<Member<double>> Vd;
    Vd v2;

    v2.x = 3.;
    v2.y() = 5;

    print( v2, &Vd::x );
    print( v2, &Vd::y );
    print( v2, &Vd::getx );
    print( v2, &Vd::gety );

    cin.get();
}