/*
 * Simple test for TimeUtils.hpp::Repeater
 */

#include <iostream>
using namespace std;

#include "TimeUtils.hpp"
using namespace Perspective;

int main()
{
    cout << "\nTPS: " << TICKS_PER_SEC << endl;

    Duration stime = ProgramTime(), ms = millisec( 1 ), s = seconds( 1 );
    Duration ctime = stime, t = ctime;

    Perspective::ElementaryTimer ET;
    Repeater<ElementaryTimer> R( &ET, s );

    while (1)
    {
        while (R.check())
        {
            cout << ProgramTime().clocks() << endl;
            //ctime +=s;
            R.repeat();
        }

        Sleep( ms );
    }

    cin.get();
    return 0;
}