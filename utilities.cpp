#include "utilities.h"

#include <random>


std::tuple<int, int> distributeParticles(const int width, const int height, const float radius) {
    // Random number generator setup
    static std::mt19937 rng(std::random_device{}()); // Random number generator
    std::uniform_int_distribution<int> distX(radius, width - radius);   // X position range: [0, width)
    std::uniform_int_distribution<int> distY(radius, height - radius);  // Y position range: [0, height)

    // Generate random X and Y positions
    int x = distX(rng);
    int y = distY(rng);

    // Return the X and Y positions as a tuple
    return std::make_tuple(x, y);
}
