#include <iomanip>
#include <iostream>
#include <valarray>
#include "Particle.h"
#include <cmath>
#include <vector>

int width = 1280;
int height = 720;

float const gravityMod = 20.0f;
float friction = 0.97f;

void CheckParticleCollision(Particle& Particle1, Particle& Particle2,
                            float const distance, const sf::Vector2f& direction) {
    if (distance <= Particle1.radius + Particle2.radius) { //collision
        //Statics
        float overlap = 0.5f * (distance - Particle1.radius - Particle2.radius);
        Particle1.position += direction * overlap;
        Particle2.position -= direction * overlap;

        //Dynamics
        sf::Vector2f tangent;
        //Tangential response
        tangent.x = -direction.y;
        tangent.y = direction.x;
        float dotproducttan1 = Particle1.velocity.x * tangent.x + Particle1.velocity.y * tangent.y;
        float dotproducttan2 = Particle2.velocity.x * tangent.x + Particle2.velocity.y * tangent.y;
        //Normal response
        float dotproductnorm1 = Particle1.velocity.x * direction.x + Particle1.velocity.y * direction.y;
        float dotproductnorm2 = Particle2.velocity.x * direction.x + Particle2.velocity.y * direction.y;
        float moment1 = (dotproductnorm1 * (Particle1.mass - Particle2.mass) + 2.0f * Particle2.mass * dotproductnorm2)
        / (Particle1.mass + Particle2.mass);
        float moment2 = (dotproductnorm2 * (Particle2.mass - Particle1.mass) + 2.0f * Particle1.mass * dotproductnorm1)
        / (Particle1.mass + Particle2.mass);

        Particle1.velocity.x = tangent.x * dotproducttan1 + direction.x * moment1;
        Particle1.velocity.y = tangent.y * dotproducttan1 + direction.y * moment1;
        Particle2.velocity.x = tangent.x * dotproducttan2 + direction.x * moment2;
        Particle2.velocity.y = tangent.y * dotproducttan2 + direction.y * moment2;




    }
}

std::tuple<float, sf::Vector2f, float> computeForce2Particles(const Particle& Particle1, const Particle& Particle2) {
    //Returns force, direction and distance
    sf::Vector2f direction = Particle2.position - Particle1.position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0) {
        direction /= distance; //Normalize vector to a unit vector of 1
    }
    float force = ((Particle1.mass * Particle2.mass) * ::gravityMod) / pow(distance, 2);
    return std::make_tuple(force, direction, distance);
}
void drawLineBetweenParticles(sf::RenderWindow& window, const Particle& particle1, const Particle& particle2) {
    // Create a VertexArray with two points
    sf::VertexArray line(sf::Lines, 2);

    // Set the position of each vertex to the origin of each particle
    line[0].position = particle1.position;
    line[1].position = particle2.position;

    // Set the color of the line (for debugging, bright colors are easier to see)
    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Red;

    // Draw the line on the window
    window.draw(line);
}

int main() {

unsigned long x = 1;
sf::RenderWindow window(sf::VideoMode(width, height), "ParticleSim");
window.setFramerateLimit(60);

// Create the circle shape once, outside the main loop
const int blueParticleCount = 30;
const int redParticleCount = 2;
const int greenParticleCount = 0;

std::vector<Particle> particles;
particles.reserve(blueParticleCount+redParticleCount+greenParticleCount);    // Reserve space to avoid reallocation

for (int i = 0; i < blueParticleCount; ++i) {
    Particle BlueCircle(std::make_tuple(0.0f, 0.0f), std::make_tuple(150.0f + i * 50.0f, 300.0f + i * 30.0f),
        20.0f, 20.f,std::make_tuple(50, 55, 255));
    particles.push_back(BlueCircle);
}
for (int i = 0; i < redParticleCount; ++i) {
    Particle RedCircle(std::make_tuple(0.0f, 0.0f), std::make_tuple(200.0f + i * 50, 200.0f), 100.0f, 50.f,
std::make_tuple(255, 55, 55));
    particles.push_back(RedCircle);
}
for (int i = 0; i < greenParticleCount; ++i) {
    Particle GreenCircle(std::make_tuple(25.0f, 0.0f), std::make_tuple(600.0f, 150.0f), 300.0f, 60.f,
    std::make_tuple(50, 255, 55));
    particles.push_back(GreenCircle);
}

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

        for (Particle& particleTo : particles) {
            if (&particleFrom == &particleTo) continue; //Skip self checking
            float force;
            float distance;

            sf::Vector2f direction;

            std::tie(force, direction, distance)  = computeForce2Particles(particleFrom, particleTo);
            netForce += force * direction;
            // drawLineBetweenParticles(window, particleFrom, particleTo);
            CheckParticleCollision(particleFrom, particleTo, distance, direction);
        }
        //Compute gravitational pull
        sf::Vector2f acc = netForce / particleFrom.mass;
        // Apply calculated speeds
        particleFrom.velocity += acc;
        particleFrom.velocity *= friction;
        particleFrom.moveby(particleFrom.velocity);
        particleFrom.checkBorderCollision(width, height);

        window.draw(particleFrom.returnObject());
    }

    window.display();

    x += 1;

}



return 0;

}
