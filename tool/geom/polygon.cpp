#include "polygon.h"

using namespace monad;

Polygon::Polygon(Vertex *list)
    : plane(list[0].pos, list[1].pos, list[2].pos) {
    for (auto i = 0; i < 3; i++) vertices[i] = list[i];
}

void Polygon::flip() {
    auto temp = vertices[2];
    vertices[2] = vertices[0];
    vertices[0] = temp;

    for (auto i = 0; i < 3; i++) {
        vertices[i].flip();
    }
}

void Polygon::transform(const Matrix4 &m) {
    transform(m, m.invert().transpose());
}

void Polygon::transform(const Matrix4 &m, const Matrix4 &n) {
    for (auto &vertex : vertices) {
        vertex.transform(m, n);
    }

    plane = Plane(vertices[0].pos, vertices[1].pos, vertices[2].pos);
}

Polygon Polygon::getTransform(const Matrix4 &m, const Matrix4 &n) const {
    auto p = *this;
    p.transform(m, n);
    return p;
}

Polygon operator*(const Matrix4 &m, Polygon p) {
    p.transform(m);
    return p;
}
