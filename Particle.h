

#ifndef PARTICLE_H
#define PARTICLE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <tuple>

class Particle {
public:
    Particle(std::tuple<float, float> invelocity, std::tuple<float, float> inposition,
        float inmass, float inradius, std::tuple<int, int, int> incolor, char ingroup,
        int intoGreen, int intoRed, int intoBlue, int intoYellow); //constructor

    sf::Vector2f velocity;
    sf::Vector2f position;
    float const mass;
    float const radius;
    char const group;
    int const toGreen; //1 means attract, 0 means ignore, -1 means repel
    int const toRed;
    int const toBlue;
    int const toYellow;


    sf::CircleShape returnObject() const;
    void moveby(sf::Vector2f movement);
    void checkBorderCollision(const int& width, const int& height);
    void CheckParticleCollision(Particle& ParticleTo, float const distance, const sf::Vector2f& direction);
    std::tuple<float, sf::Vector2f, float> computeForce2Particles(const Particle& ParticleTo, const float gravityMod);
    sf::Vector2f netForce(const Particle& particleTo, sf::Vector2f& netForce, const float force, sf::Vector2f direction);


private:
    sf::Color const color;
    sf::CircleShape object;
};

#endif //PARTICLE_H
