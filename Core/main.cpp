/*
 * File: main.cpp
 * Created: November 5, 2015
 * Contains: Main loop
 */

//------------------------ Standart includes -------------------------
#include <iostream>

//------------------------ External includes -------------------------
#include <SFML/Graphics.hpp>

//------------------------ Project includes --------------------------
#include "CoreTypes.hpp"
#include "Timer.hpp"
#include "TimeUtils.hpp"

//------------------------------- MAIN -------------------------------
using namespace Perspective;


int main()
{

    sf::RenderWindow window( sf::VideoMode( 200, 200 ), "SFML works!" );
    sf::CircleShape shape( 100.f );
    shape.setFillColor( sf::Color::Green );
    sf::Clock clock;
    sf::Time time, sec = sf::seconds( 1. );

    while (window.isOpen())  // <- Actually the main loop
    {
        sf::Event event;
        while (window.pollEvent( event ))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time t = clock.getElapsedTime();
        if (t > sec + time)
        {
            time = t;
            printf( "Time: %d:%d\n", ((int)time.asSeconds()) / 60, (int)time.asSeconds() );
        }

        window.clear();
        window.draw( shape );
        window.display();
    }

    return 0;
}