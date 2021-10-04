#pragma once

#include "vector.h"
#include "list.h"

namespace monad {

    class Polygon;

    class Plane {
    public:
        static constexpr float EPSILON = 0.00001f;

        vec3 normal;
        float w;

        Plane(const vec3 &normal, float w);
        Plane(const vec3 &a, const vec3 &b, const vec3 &c);

        void flip();
        void splitPolygon(Polygon &polygon, List<Polygon> &coplanarFront, List<Polygon> &coplanarBack, List<Polygon> &front, List<Polygon> &back);
    };

}
