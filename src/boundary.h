#pragma once

#include "vectorn.h"

struct Boundary {
    virtual ~Boundary() = default;

    virtual VectorN bounce(const VectorN& other) const = 0;
    virtual std::size_t dimensions() const = 0;
    virtual VectorN center() const = 0;
    virtual float surface() const = 0;
    virtual float volume() const = 0;
    virtual VectorN randomPoint() const = 0;
};

struct CubeBoundary : Boundary {
    VectorN size;
    VectorN offset;
    CubeBoundary(VectorN size, VectorN offset);
    VectorN bounce(const VectorN &other) const override;
    std::size_t dimensions() const override;
    VectorN center() const override;
    float surface() const override;
    float volume() const override;
    VectorN randomPoint() const override;
};

