#pragma once
#include "vertex.h"
#include "plane.h"

namespace monad {

    class Polygon {
    public:
        Vertex vertices[3];
        Plane plane;

        explicit Polygon(Vertex list[3]);
        void flip();
        void transform(const Matrix4 &m);
        void transform(const Matrix4 &m, const Matrix4 &n);
        Polygon getTransform(const Matrix4 &m, const Matrix4 &n) const;
    };

    Polygon operator*(const Matrix4 &m, Polygon p);

}