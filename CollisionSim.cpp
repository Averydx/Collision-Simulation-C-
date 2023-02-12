// CollisionSim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Particle.h"
#include "SFML/Graphics.hpp"
#include "SFML_VECTOR.h"
#include <time.h>
#include <stdlib.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Particle Collision Simulation");
    int x_size = window.getSize().x; 
    int y_size = window.getSize().y; 
    srand(time(NULL));
    //Particle 

    std::vector<Particle>* particles = new std::vector<Particle>();
    for (int i = 0; i < 100; i++)
    {
        
        Vector2D pos(rand() % 600, rand() % 400); 
        Vector2D vel(rand() % 5, rand() % 5);
        float mass = rand() % 5;  
        float radius = sqrt(mass) * 3; 
        sf::Color color;
        switch (rand() % 4)
        {
        case(0):
            color = sf::Color::Blue; 
            break; 
        case(1):
            color = sf::Color::Red; 
            break; 

        case(2):
            color = sf::Color::Green; 
            break; 

        default: 
            color = sf::Color::Yellow; 
            break; 
        }
         

        particles->push_back(Particle(pos, vel, color, radius, mass,x_size,y_size));
    }

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i position = sf::Mouse::getPosition();
                Vector2D pos(position.x, position.y);
                sf::Color color;
                switch (rand() % 4)
                {
                case(0):
                    color = sf::Color::Blue;
                    break;
                case(1):
                    color = sf::Color::Red;
                    break;

                case(2):
                    color = sf::Color::Green;
                    break;

                default:
                    color = sf::Color::Yellow;
                    break;
                }

                particles->push_back(Particle(pos, Vector2D(5,5),color,sqrt(20)*3, 20,x_size,y_size));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }


        window.clear();

        Vector2D wind(0.3, 0); 
        Vector2D g(0, 0.1);
        double c = 0.03; 
        for (int i = 0; i < particles->size(); i++)
        {
            //particles->at(i).apply_force(wind);
            //particles->at(i).apply_force(g * particles->at(i).getMass());
            particles->at(i).apply_force((-1 * c) * particles->at(i).getVel());
            particles->at(i).update_physics();
            particles->at(i).render(window);
            for (int j = 0; j < particles->size(); j++)
            {
                if (i != j) 
                {
                    particles->at(i).collision_check(particles->at(j));
                    
                }
            }

        }


        
        
        
        window.display();
    }

}

