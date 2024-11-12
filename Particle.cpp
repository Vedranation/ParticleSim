
#include "Particle.h"

Particle::Particle(std::tuple<float, float> invelocity, std::tuple<float, float> inposition,
        float inmass, float inradius, std::tuple<int, int, int> incolor)
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