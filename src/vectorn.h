#pragma once

#include <vector>

struct VectorN {
    VectorN(std::initializer_list<float> values);

    std::vector<float> values;

    explicit VectorN(std::size_t dimensions);

    std::size_t dimensions() const;

    float& operator[](std::size_t i);

    const float& operator[](std::size_t i) const;

    VectorN operator+(const VectorN& other) const;

    VectorN operator*(const float scale) const;

    float length_squared() const;

    float length() const;

    VectorN operator-() const;

    VectorN normalized() const;
};
