/*
 * Simple tests for TimeUtils.hpp::Sleep()
 */

#include <iostream>
using namespace std;

#include "TimeUtils.hpp"
using namespace Perspective;

Duration ms = millisec( 1 ), s = seconds( 1 );

void foo1( int id )
{
    Duration p = seconds( 0.2 ), t, l = ProgramTime();
    while (1)
    {
        if ((t = ProgramTime()) > l + p)
        {
            cout << id << endl;
            l += s;
        }
        Sleep( ms );
    }
}

void foo2( int id )
{
    Duration p = seconds( 0.5 ), t, l = ProgramTime();
    while (1)
    {
        if ((t = ProgramTime()) > l + p)
        {
            cout << id << endl;
            l += s;
        }
        Sleep( ms );
    }
}

int main()
{
    std::thread first( foo1, 1 );
    std::thread seconf( foo2, 2 );

    Duration l = ProgramTime();
    while (1)
    {
        if ((T = ProgramTime()) > l + s)
        {
            cout << 0 << "\t" << ProgramTime() << endl;
            l += s;
        }
        Sleep( ms );
    }

    return 0;
}