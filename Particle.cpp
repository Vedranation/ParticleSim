
#include "Particle.h"

Particle::Particle(std::tuple<float, float> invelocity, std::tuple<float, float> inposition,
        float inmass, float inradius, std::tuple<int, int, int> incolor)
    //variable initialization
    : velocity{std::get<0>(invelocity), std::get<1>(invelocity)},
      position{std::get<0>(inposition), std::get<1>(inposition)},
      color{sf::Uint8(std::get<0>(incolor)), sf::Uint8(std::get<1>(incolor)), sf::Uint8(std::get<2>(incolor))},
      mass{inmass},
      radius{inradius},
      object{radius}

    {
        // Additional constructor logic, if needed
    object.setFillColor(color);
    object.setOrigin(radius, radius);
    object.setPosition(position);
    }

sf::CircleShape Particle::returnObject() {
    return object;
}

void Particle::moveby(sf::Vector2f movement) {
    position += movement;
    object.move(movement);
}

void Particle::checkBorderCollision(const int &width, const int &height) {

    float threshhold = 0.00f;
    if (position.x - radius < 0.0f) { //Left wall
        velocity.x = -velocity.x;
        position.x = radius * (1.0f + threshhold); //Snap it back inside the playground
        return;
    }
    else if (position.x + radius > width) { //Right wall
        velocity.x = -velocity.x;
        position.x = (width - radius) * (1.0f - threshhold);
        return;
    }

    if (position.y - radius < 0.0f) { //Top wall
        velocity.y = -velocity.y;
        position.y = radius * (1.0f + threshhold);
        return;
    }
    else if (position.y + radius > height) { //Bottom wall
        velocity.y = -velocity.y;
        position.y = (height - radius) * (1.0f - threshhold);
        return;
    }
}
