#include <iomanip>
#include <iostream>
#include <valarray>
#include "Particle.h"
#include <cmath>
#include <vector>

int width = 1280;
int height = 720;

float const gravityMod = 50.0f;
float friction = 0.99f;

void CheckParticleCollision(const int& redRadius, const int& greenRadius, const sf::Vector2f& redPos,
                            const sf::Vector2f& greenPos, sf::Vector2f& redVel, sf::Vector2f& greenVel,
                            float const distance, const sf::Vector2f& G2RdirectionNorm) {
    if (distance <= redRadius + greenRadius) {//collision

        sf::Vector2f relativeVel = redVel - greenVel;
        float velocityNormal = relativeVel.x * G2RdirectionNorm.x + relativeVel.y * G2RdirectionNorm.y;


            //Particles moving toward each other
            redVel = redVel + -1.0f * G2RdirectionNorm;
            greenVel = greenVel + -1.0f * G2RdirectionNorm;


    }
}

std::tuple<float, sf::Vector2f, float> computeForce2Particles(Particle Particle1, Particle Particle2) {
    //Returns force, direction and distance
    sf::Vector2f direction = Particle2.position - Particle1.position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0) {
        direction /= distance; //Normalize vector to a unit vector of 1
    }
    float force = ((Particle1.mass * Particle2.mass) * ::gravityMod) / pow(distance, 2);
    return std::make_tuple(force, direction, distance);
}


int main() {


sf::RenderWindow window(sf::VideoMode(width, height), "ParticleSim");
window.setFramerateLimit(60);

// Create the circle shape once, outside the main loop

Particle BlueCircle(std::make_tuple(0.0f, 0.0f), std::make_tuple(300.0f, 300.0f), 20.0f, 50.f,
    std::make_tuple(50, 55, 255));
Particle GreenCircle(std::make_tuple(25.0f, 0.0f), std::make_tuple(100.0f, 150.0f), 20.0f, 50.f,
    std::make_tuple(50, 255, 55));
Particle RedCircle(std::make_tuple(0.0f, 0.0f), std::make_tuple(200.0f, 200.0f), 20.0f, 50.f,
std::make_tuple(255, 55, 55));

std::vector<Particle> particles = {BlueCircle, GreenCircle, RedCircle};

while (window.isOpen()) {

    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseMoved:

            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                std::cout << "Mouse Left button pressed\n";
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left) {
                std::cout << "Mouse Left button released\n";
            }
            break;
    }
    }
    window.clear(sf::Color::Black);

    for (Particle& particleFrom : particles) {
        sf::Vector2f netForce{0.0f, 0.0f};

        for (const Particle& particleTo : particles) {
            if (&particleFrom == &particleTo) continue; //Skip self checking
            float force;
            float distance;

            sf::Vector2f direction;

            std::tie(force, direction, distance)  = computeForce2Particles(particleFrom, particleTo);
            netForce += force * direction;
        }
        //Compute gravitational pull
        sf::Vector2f acc = netForce / particleFrom.mass;
        // Apply calculated speeds
        particleFrom.velocity += acc;
        particleFrom.velocity *= friction;
        particleFrom.checkBorderCollision(width, height);
        particleFrom.moveby(particleFrom.velocity);
        window.draw(particleFrom.returnObject());
    }

    // CheckParticleCollision(RedCircle.getRadius(), GreenCircle.getRadius(), RedPos, GreenPos, red_velocity,
        // green_velocity, distance, G2Rdirection);

    window.display();
    // std::cout << std::fixed << std::setprecision(5)  // Set precision to 5 decimal places
    //       << "X: " << RedCircle.position.x - RedCircle.returnObject().getPosition().x
    //       << " Y: " << RedCircle.position.y - RedCircle.returnObject().getPosition().y
    //       << std::endl;
    int x = 1;

}



return 0;

}
