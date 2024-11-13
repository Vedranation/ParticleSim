
#include "Particle.h"

#include <cmath>

Particle::Particle(std::tuple<float, float> invelocity, std::tuple<float, float> inposition,
                   float inmass, float inradius, std::tuple<int, int, int> incolor, char const ingroup,
                   int const intoGreen, int const intoRed, int const intoBlue, int const intoYellow)
    //variable initialization
    : velocity{std::get<0>(invelocity), std::get<1>(invelocity)},
      position{std::get<0>(inposition), std::get<1>(inposition)},
      color{sf::Uint8(std::get<0>(incolor)), sf::Uint8(std::get<1>(incolor)), sf::Uint8(std::get<2>(incolor))},
      mass{inmass},
      radius{inradius},
      group{ingroup},
      toGreen{intoGreen},
      toRed{intoRed},
      toBlue{intoBlue},
      toYellow{intoYellow},
      object{radius}


    {
        // Additional constructor logic, if needed
    object.setFillColor(color);
    object.setOrigin(radius, radius);
    object.setPosition(position);
    }

sf::CircleShape Particle::returnObject() const {
    return object;
}

void Particle::moveby(sf::Vector2f movement) {
    position += movement;
    object.setPosition(position);
}

void Particle::checkBorderCollision(const int &width, const int &height, const float& collisionLoss) {

    float threshhold = 0.00f;
    if (position.x - radius < 0.0f) { //Left wall
        velocity.x = -velocity.x * collisionLoss;
        velocity.y = velocity.y * collisionLoss;
        position.x = radius * (1.0f + threshhold); //Snap it back inside the playground
    }
    else if (position.x + radius > width) { //Right wall
        velocity.x = -velocity.x * collisionLoss;
        velocity.y = velocity.y * collisionLoss;
        position.x = (width - radius) * (1.0f - threshhold);
    }

    if (position.y - radius < 0.0f) { //Top wall
        velocity.y = -velocity.y * collisionLoss;
        velocity.x = velocity.x * collisionLoss;
        position.y = radius * (1.0f + threshhold);
    }
    else if (position.y + radius > height) { //Bottom wall
        velocity.y = -velocity.y;
        velocity.x = velocity.x * collisionLoss;
        position.y = (height - radius) * (1.0f - threshhold);
    }
}

void Particle::CheckParticleCollision(Particle& ParticleTo, float const distance, const sf::Vector2f& direction,
    const float& collisionLoss) {
    if (distance <= radius + ParticleTo.radius) { //collision
        //Statics
        float overlap = (distance - radius - ParticleTo.radius);
        float mass_diff1 = mass / (mass + ParticleTo.mass);
        float mass_diff2 = ParticleTo.mass / (mass + ParticleTo.mass);
        position += direction * overlap * mass_diff2;
        ParticleTo.position -= direction * overlap * mass_diff1;

        //Dynamics
        sf::Vector2f tangent;
        //Tangential response
        tangent.x = -direction.y;
        tangent.y = direction.x;
        float dotproducttan1 = velocity.x * tangent.x + velocity.y * tangent.y;
        float dotproducttan2 = ParticleTo.velocity.x * tangent.x + ParticleTo.velocity.y * tangent.y;
        //Normal response
        float dotproductnorm1 = velocity.x * direction.x + velocity.y * direction.y;
        float dotproductnorm2 = ParticleTo.velocity.x * direction.x + ParticleTo.velocity.y * direction.y;
        float moment1 = (dotproductnorm1 * (mass - ParticleTo.mass) + 2.0f * ParticleTo.mass * dotproductnorm2)
        / (mass + ParticleTo.mass);
        float moment2 = (dotproductnorm2 * (ParticleTo.mass - mass) + 2.0f * mass * dotproductnorm1)
        / (mass + ParticleTo.mass);

        velocity.x = collisionLoss * (tangent.x * dotproducttan1 + direction.x * moment1);
        velocity.y = collisionLoss * (tangent.y * dotproducttan1 + direction.y * moment1);
        ParticleTo.velocity.x = collisionLoss * (tangent.x * dotproducttan2 + direction.x * moment2);
        ParticleTo.velocity.y = collisionLoss * (tangent.y * dotproducttan2 + direction.y * moment2);

    }
}

std::tuple<float, sf::Vector2f, float> Particle::computeForce2Particles(const Particle& ParticleTo, const float gravityMod) {
    //Returns force, direction and distance
    sf::Vector2f direction = ParticleTo.position - position;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0) {
        direction /= distance; //Normalize vector to a unit vector of 1
    }
    float force = ((mass * ParticleTo.mass) * gravityMod) / std::pow(distance, 2);
    return std::make_tuple(force, direction, distance);
}

sf::Vector2f Particle::netForce(const Particle& particleTo, sf::Vector2f& netForce, const float force, sf::Vector2f direction) {
    if (particleTo.group == 'b'){
        if (toBlue == 1) {
            netForce += force * direction; //Attraction
            return netForce;
        }
        if (toBlue == 0) {
            return netForce; //no change in netForce
        }
        netForce -= force * direction; //Inverse polarity
        return netForce;
    }
    if (particleTo.group == 'r') {
        if (toRed == 1) {
            netForce += force * direction; //Attraction
            return netForce;
        }
        if (toRed == 0) {
            return netForce;
        }
        netForce -= force * direction; //Inverse polarity
        return netForce;
    }
    if (particleTo.group == 'g') {
        if (toGreen == 1) {
            netForce += force * direction; //Attraction
            return netForce;
        }
        if (toGreen == 0) {
            return netForce; //no change in netForce
        }
        netForce -= force * direction; //Inverse polarity
        return netForce;
    }
    if (particleTo.group == 'y') {
        if (toYellow == 1) {
            netForce += force * direction; //Attraction
            return netForce;
        }
        if (toYellow == 0) {
            return netForce; //no change in netForce
        }
        netForce -= force * direction; //Inverse polarity
        return netForce;
    }
}
