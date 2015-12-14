/* 
 * Simple test for Time.hpp
 */

#include <iostream>
using namespace std;

#include "Time.hpp"
using namespace Perspective;

int main()
{
    std::cout << "Start moment: " << StartMoment.as_c_str() << endl << endl;

    int64_t a = 0, b;
    std::cout << "enter amount of loops:   ";
    std::cin >> b;
    std::cout << "calculating...\n";

    Time t1 = SystemTime();
    Duration dt1 = ProgramTime();
    for (int64_t i = 0; i<b; i++)
    {
        a += i;
    }
    Time t2 = SystemTime();
    Duration dt2 = ProgramTime();
    Duration deltatime( dt2 - dt1 );

    std::cout << "dt=deltatime:  " << deltatime << std::endl;
    std::cout << "dt(millisec):  " << deltatime.asMilliSec() << std::endl;
    std::cout << "System time 1: " << t1.as_c_str() << std::endl;
    std::cout << "System time 2: " << t2.as_c_str() << std::endl;
    std::cout << "SysT2 - SysT1: " << t2 - t1 << std::endl;
    std::cout << "SysT2 + 10*dt: " << (t2 + 10. * deltatime).as_c_str() << std::endl << std::endl;
    std::cout << "dt1 (clocks) : " << dt1.getTicks() << std::endl;
    std::cout << "dt2 (clocks) : " << dt2.getTicks() << std::endl;
    std::cout << "SysT1(clocks): " << t1.getTicks() << std::endl;
    std::cout << "SysT2(clocks): " << t2.getTicks() << std::endl;

    std::cin.get();
    std::cin.get();
}