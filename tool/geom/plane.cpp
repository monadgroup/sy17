#include "plane.h"
#include "polygon.h"

using namespace monad;

Plane::Plane(const vec3 &normal, float w)  : normal(normal), w(w) { }

Plane::Plane(const vec3 &a, const vec3 &b, const vec3 &c) : normal((b - a).cross(c - a).unit()), w(normal.dot(a)) { }

void Plane::flip() {
    normal = -normal;
    w = -w;
}

void Plane::splitPolygon(Polygon &polygon, List<Polygon> &coplanarFront, List<Polygon> &coplanarBack,
                         List<Polygon> &front, List<Polygon> &back) {
    enum {
        COPLANAR = 0,
        FRONT = 1,
        BACK = 2,
        SPANNING = 3
    };

    int polygonType = 0;
    List<int> types;

    for (const auto &vertice : polygon.vertices) {
        auto t = normal.dot(vertice.pos) - w;
        auto type = (t < -EPSILON) ? BACK : ((t > EPSILON) ? FRONT : COPLANAR);
        polygonType |= type;
        types.push_back(type);
    }

    switch (polygonType) {
        case COPLANAR:
        {
            if (normal.dot(polygon.plane.normal) > 0) coplanarFront.push_back(polygon);
            else coplanarBack.push_back(polygon);
            break;
        }
        case FRONT:
        {
            front.push_back(polygon);
            break;
        }
        case BACK:
        {
            back.push_back(polygon);
            break;
        }
        case SPANNING:
        {
            List<Vertex> f, b;
            for (size_t i = 0; i < 3; i++) {
                auto j = (i + 1) % 3;
                auto ti = types[i], tj = types[j];
                auto vi = polygon.vertices[i], vj = polygon.vertices[j];
                if (ti != BACK) f.push_back(vi);
                if (ti != FRONT) b.push_back(vi);
                if ((ti | tj) == SPANNING) {
                    auto t = (w - normal.dot(vi.pos)) / normal.dot(vj.pos - vi.pos);
                    auto v = vi.interpolate(vj, t);
                    f.push_back(v);
                    b.push_back(v);
                }
            }

            if (f.size() >= 3) front.push_back(Polygon(f.begin()));
            if (b.size() >= 3) back.push_back(Polygon(b.begin()));

            break;
        }
    }
}

