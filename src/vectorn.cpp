#include "vectorn.h"

#include <algorithm>
#include <cmath>
#include <initializer_list>

VectorN::VectorN(std::initializer_list<float> values) : values(values) {}

VectorN::VectorN(std::size_t i) : values(i, 0.0) {}

std::size_t VectorN::dimensions() const {
    return values.size();
}

float& VectorN::operator[](std::size_t i) {
    return values[i];
}

const float& VectorN::operator[](std::size_t i) const {
    return values[i];
}

VectorN VectorN::operator+(const VectorN& other) const {
    VectorN result(std::min(dimensions(), other.dimensions()));
    for (std::size_t i = 0; i < std::min(dimensions(), other.dimensions()); ++i)
        result[i] = values[i] + other[i];

    return result;
}

float VectorN::length_squared() const {
    float length_squared = 0;
    for (float value : this->values) {
        length_squared += value * value;
    }
    return length_squared;
}

float VectorN::length() const {
    return std::sqrt(this->length_squared());
}

VectorN VectorN::operator-() const {
    VectorN result(values.size());

    for (std::size_t i = 0; i < values.size(); ++i)
        result.values[i] = -values[i];

    return result;
}

VectorN VectorN::operator*(const float scale) const {
    VectorN scaled(values.size());
    for (std::size_t i = 0; i < values.size(); i++) {
        scaled[i] = values[i]*scale;
    }
    return scaled;
}

VectorN VectorN::normalized() const {
    float len = length();

    if (len == 0.0f) {
        return VectorN(dimensions());
    }

    VectorN result = *this;

    for (std::size_t i = 0; i < dimensions(); ++i) {
        result[i] /= len;
    }

    return result;
}
