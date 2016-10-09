
#include <iostream>
using namespace std;

#include "ConstexprCounter.hpp"
using namespace ConstexprNmspc;

// ==========================================================================

template<int i>
void Print() { cout << i << endl; }

// ==========================================================================

int main ()
{
 
    Print<count()>();
    Print<count()>();
    Print<count()>();
    Print<count()>();
    cout << endl;
    init<10, 2>();
    Print<count()>();
    Print<count()>();
    //init<10, 2>();
    //init<10, 3>();
    Print<count()>();
    Print<count()>();

    cout << endl;
    cout << endl;

    Print<observe<1>()>();
    Print<count<1>()>();
    Print<observe<1>()>();
    Print<count<1>()>();
    Print<observe<1>()>();

    cout << endl;
    cout << endl;

    Print<observe<int>()>();
    Print<init<-10, -3, int>()>();
    Print<count<int>()>();
    Print<count<int>()>();
    Print<observe<int>()>();

    cout << endl;
    cout << endl;

    Print<getStep<TagType<>>()>();
    Print<getStep<TagType<1>>()>();
    Print<getStep<int>()>();

    cout << endl;

    Print<getInit<>()>();
    Print<getInit<1>()>();
    Print<getInit<int>()>();

    cin.get();
}
