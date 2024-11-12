

#ifndef PARTICLE_H
#define PARTICLE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <tuple>

class Particle {
public:
    Particle(std::tuple<float, float> invelocity, std::tuple<float, float> inposition,
        float inmass, float inradius, std::tuple<int, int, int> incolor); //constructor

    sf::Vector2f velocity;
    sf::Vector2f position;
    float const mass;
    float const radius;



private:
    sf::Color const color;
    sf::CircleShape object;
};

#endif //PARTICLE_H