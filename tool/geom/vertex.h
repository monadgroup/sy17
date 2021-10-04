#pragma once
#include "vector.h"

namespace monad {

    class Vertex {
    public:
        vec3 pos;
        vec3 normal;
        float material;

        Vertex();
        Vertex(vec3 pos, vec3 normal, float material);

        void flip();
        void transform(const Matrix4 &m);
        void transform(const Matrix4 &m, const Matrix4 &n);
        Vertex getTransform(const Matrix4 &m, const Matrix4 &n);
        Vertex interpolate(Vertex &other, float t) const;
    };

    Vertex operator*(const Matrix4 &m, Vertex v);

}
