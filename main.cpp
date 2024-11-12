#include <iomanip>
#include <iostream>
#include <valarray>
#include "Particle.h"
#include <cmath>

bool CheckBorderCollision(const int& width, const int& height, sf::Vector2f pos, sf::Vector2f& vel, const int radius) {
    if (pos.x - radius < 0.0f) { //Left wall
        vel.x = -vel.x;
        return true;
    }
    else if (pos.x + radius > width) { //Right wall
        vel.x = -vel.x;
        return true;
    }

    if (pos.y - radius < 0.0f) { //Top wall
        vel.y = -vel.y;
        return true;
    }
    else if (pos.y + radius > height) { //Bottom wall
        vel.y = -vel.y;
        return true;
    }

    return false;
}

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



int main() {
int width = 1280;
int height = 720;
sf::RenderWindow window(sf::VideoMode(width, height), "ParticleSim");
window.setFramerateLimit(60);

// Create the circle shape once, outside the main loop
sf::CircleShape GreenCircle(20.f);
GreenCircle.setFillColor(sf::Color(50, 255, 55));
GreenCircle.setPosition(400.f, 250.f);
GreenCircle.setOrigin(GreenCircle.getRadius(), GreenCircle.getRadius());

sf::CircleShape RedCircle(20.f);
RedCircle.setFillColor(sf::Color(250, 55, 55));
RedCircle.setPosition(250.f, 200.f);
RedCircle.setOrigin(RedCircle.getRadius(), RedCircle.getRadius());

float const gravityMod = 50.0f;
float friction = 0.99f;
const float green_mass = 20.0f;
const float red_mass = 20.0f;

sf::Vector2f green_velocity(0.0f, 0.0f);
sf::Vector2f red_velocity(0.0f, 0.0f);

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

    sf::Vector2f GreenPos = GreenCircle.getPosition();
    sf::Vector2f RedPos = RedCircle.getPosition();
    sf::Vector2f G2Rdirection = RedPos - GreenPos;
    float distance = std::sqrt(G2Rdirection.x * G2Rdirection.x + G2Rdirection.y * G2Rdirection.y);
    if (distance > 0) {
        G2Rdirection /= distance; //Normalize vector to a unit vector of 1
    }

    //Compute gravitational pull
    float Force = ((green_mass * red_mass) * gravityMod) / pow(distance, 2);
    float red_acc = Force / red_mass;
    float green_acc = Force / green_mass;

    if (distance > 2) { //prevent division by small number singulary
        green_velocity = green_velocity + green_acc * G2Rdirection;
        red_velocity = red_velocity + red_acc * -G2Rdirection;
    }

    CheckBorderCollision(width, height, RedPos, red_velocity, RedCircle.getRadius());
    CheckBorderCollision(width, height, GreenPos, green_velocity, GreenCircle.getRadius());
    // CheckParticleCollision(RedCircle.getRadius(), GreenCircle.getRadius(), RedPos, GreenPos, red_velocity,
        // green_velocity, distance, G2Rdirection);

    green_velocity *= friction;
    red_velocity *= friction;

    GreenCircle.move(green_velocity);
    RedCircle.move(red_velocity);



    window.draw(GreenCircle);
    window.draw(RedCircle);
    window.display();
    int x = 1;

}



return 0;

}
