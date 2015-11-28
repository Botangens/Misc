/*
* File: main.cpp
* Created: November 5, 2015
* Contains: Main loop
*/

//------------------------ standart includes -------------------------
#include <iostream>

//------------------------ external includes -------------------------
#include <SFML/Graphics.hpp>

//------------------------ project includes --------------------------
#include "CoreTypes.hpp"
#include "Timer.hpp"
#include "TimeUtils.hpp"

//------------------------------- MAIN -------------------------------
using namespace Perspective;

int main()
{
    int a = 0, b;
    std::cout << "enter amount of loops:   ";
    std::cin >> b;
    std::cout << "calculating...\n";

    Time t1 = SystemTime();
    Duration dt1 = ProgramTime();
    for (int i = 0; i<b; i++)
    {
        a += i;
    }
    Time t2 = SystemTime();
    Duration dt2 = ProgramTime();
    Duration deltatime(dt2 - dt1);

    std::cout << "dt=deltatime:  " << deltatime << std::endl;
    std::cout << "dt(millisec):  " << deltatime.asMilliSec() << std::endl;
    std::cout << "System time 1: " << t1.as_c_str() << std::endl;
    std::cout << "System time 2: " << t2.as_c_str() << std::endl;
    std::cout << "SysT2 - SysT1: " << t2 - t1 << std::endl;
    std::cout << "SysT2 + 10*dt: " << (t2 + 10. * deltatime).as_c_str() << std::endl << std::endl;
    std::cout << "dt1 (clocks) : " << dt1.showTicks() << std::endl;
    std::cout << "dt2 (clocks) : " << dt2.showTicks() << std::endl;
    std::cout << "SysT1(clocks): " << t1.showTicks() << std::endl;
    std::cout << "SysT2(clocks): " << t2.showTicks() << std::endl;


	std::cin.get();
    std::cin.get();
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	sf::Clock clock;
	sf::Time time, sec = sf::seconds(1.);

	while (window.isOpen())  // <- Actually the main loop
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		sf::Time t = clock.getElapsedTime();
		if (t > sec + time)
		{
			time = t;
			printf("Time: %d:%d\n", ((int)time.asSeconds()) / 60, (int)time.asSeconds() );
		}
		
		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}