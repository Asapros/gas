#include <random>

namespace {
    std::mt19937 rng(std::random_device{}());
}

float randomFloat(float x, float y) {
    return std::uniform_real_distribution<float>(x, y)(rng);
}
