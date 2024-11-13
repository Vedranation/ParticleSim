#include <iomanip>
#include <iostream>
#include <valarray>
#include "Particle.h"
#include "utilities.h"
#include <vector>

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

    int width = 1920;
    int height = 1080;

    float const gravityMod = 50.0f;
    float friction = 0.97f;
    const int blueParticleCount = 120;
    const int redParticleCount = 3;
    const int greenParticleCount = 200;
    const int yellowParticleCount = 1;

sf::RenderWindow window(sf::VideoMode(width, height), "ParticleSim");
window.setFramerateLimit(60);


std::vector<Particle> particles;
particles.reserve(blueParticleCount+redParticleCount+greenParticleCount+yellowParticleCount);    // Reserve space to avoid reallocation

for (int i = 0; i < blueParticleCount; ++i) {
    float radius = 10.f;
    Particle BlueCircle(std::make_tuple(0.0f, 0.0f), distributeParticles(width, height, radius),
        1.f, radius,std::make_tuple(50, 55, 255), 'b', -1, 1, 1, 1);
    particles.push_back(BlueCircle);
}
for (int i = 0; i < redParticleCount; ++i) {
    float radius = 50.f;
    Particle RedCircle(std::make_tuple(0.0f, 0.0f), distributeParticles(width, height, radius),
        100.0f, radius, std::make_tuple(255, 55, 55), 'r', 1, -1, 1, 1);
    particles.push_back(RedCircle);
}
for (int i = 0; i < greenParticleCount; ++i) {
    float radius = 5.f;
    Particle GreenCircle(std::make_tuple(0.0f, 0.0f), distributeParticles(width, height, radius),
        0.1f, radius,std::make_tuple(50, 255, 55), 'g', 1, 1, -1, 1);
    particles.push_back(GreenCircle);
}
for (int i = 0; i < yellowParticleCount; ++i) {
    float radius = 80.f;
    Particle YellowCircle(std::make_tuple(0.0f, 0.0f), distributeParticles(width, height, radius),
        400.f, radius,std::make_tuple(255, 255, 55), 'y', 1, 1, 1, -1);
    particles.push_back(YellowCircle);
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

            std::tie(force, direction, distance)  = particleFrom.computeForce2Particles(particleTo, gravityMod);

            particleFrom.netForce(particleTo, netForce, force, direction); //This applies attract or repel forces

            // drawLineBetweenParticles(window, particleFrom, particleTo);
            particleFrom.CheckParticleCollision(particleTo, distance, direction);
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
