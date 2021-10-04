#pragma once

#include "vector.h"

namespace monad {
    class Mesh;
}

namespace gen {

    void quad(monad::Mesh *mesh, float material, const vec2 &size, const Matrix4 &transform = Matrix4::getIdentity());
    void box(monad::Mesh *mesh, float material, const vec3 &size, const Matrix4 &transform = Matrix4::getIdentity());
    void sphere(monad::Mesh *mesh, float material, float radius, const Matrix4 &transform = Matrix4::getIdentity(), int slices = 16, int stacks = 8);
    void cylinder(monad::Mesh *mesh, float material, float height, vec2 radii, const Matrix4 &transform = Matrix4::getIdentity(), int slices = 16);
}
