#include "vectorn.h"
#include <random>
#include <raylib.h>

namespace {
    std::mt19937 rng(std::random_device{}());
}

float randomFloat(float x, float y) {
    return std::uniform_real_distribution<float>(x, y)(rng);
}

VectorN randomUnitVector(std::size_t dimensions) {
    VectorN direction(dimensions);
    for (std::size_t dimension = 0; dimension < dimensions; dimension++) {
        direction[dimension] = randomFloat(-1, 1);
    }
    return direction.normalized();
}

Color randomColor() {
    return Color {
        static_cast<unsigned char>(randomFloat(50, 200)),
        static_cast<unsigned char>(randomFloat(50, 200)),
        static_cast<unsigned char>(randomFloat(50, 200)),
        255
    };
}

