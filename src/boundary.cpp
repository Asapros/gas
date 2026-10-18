#include "boundary.h"
#include "vectorn.h"
#include "rng.h"

CubeBoundary::CubeBoundary(VectorN size, VectorN offset) : size(size), offset(offset) {}

VectorN CubeBoundary::bounce(const VectorN& other) const {
    VectorN result(std::min(dimensions(), other.dimensions()));

    for (std::size_t i = 0; i < result.dimensions(); ++i) {
        if (other.values[i] < offset[i])
            result.values[i] = other.values[i] - offset[i];
        else if (other.values[i] > this->size[i] + offset[i])
            result.values[i] = other.values[i] - (this->size[i] + offset[i]);
    }
    return -result * 2;
}

std::size_t CubeBoundary::dimensions() const {
    return this->size.dimensions();
}
VectorN CubeBoundary::center() const {
    VectorN center(dimensions());
    for (std::size_t dimension = 0; dimension < this->dimensions(); dimension++) {
        center[dimension] = offset[dimension] + size[dimension]/2;
    }
    return center;
}

float CubeBoundary::surface() const {
    float surface = 0;

    for (size_t i = 0; i < dimensions(); i++) {
        float face = 1;

        for (size_t j = 0; j < dimensions(); j++) {
            if (i != j)
                face *= size.values[j];
        }

        surface += face;
    }

    return 2 * surface;
}
float CubeBoundary::volume() const {
    float volume = 1;
    for (float length : size.values) {
        volume *= length;
    }
    return volume;
}

VectorN CubeBoundary::randomPoint() const {
    VectorN point(dimensions());
    for (size_t i = 0; i < dimensions(); i++) {
        point[i] = randomFloat(this->offset[i], this->size[i] + this->offset[i]);
    }
    return point;
}
