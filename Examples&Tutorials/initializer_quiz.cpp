#include <iostream>
using namespace std;

// -------- some class with combined constructor ---------
class A
{
public:
    int i;
    int j;
    A( const int& I = 0, const int& J = 0 ) : i( I ), j( J ) {}
};

// -------- several overloaded functions with different args ---------
void foo( const A& a )
{
    cout << a.i << "\t" << a.j << endl;
}
void foo( int& i )
{
    cout << "this is INT\n";
}
void foo( double& d )
{
    cout << "this is DOUBLE\n";
}

// --- THE MAIN >:-D ---
int main()
{
    // the quiz: predict the output
    foo( 0 );
    foo( 0. );
    foo( { 0 } );
    foo( {} );

// -----------------
    cin.get();
}