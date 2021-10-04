#include "vertex.h"
#include "util.h"

using namespace monad;

Vertex::Vertex() : pos({0, 0, 0}), normal({0, 0, 0}), material(0) { }

Vertex::Vertex(vec3 pos, vec3 normal, float material)
    : pos(pos), normal(normal), material(material) { }

void Vertex::flip() {
    normal = -normal;
}

void Vertex::transform(const Matrix4 &m) {
    transform(m, m.invert().transpose());
}

void Vertex::transform(const Matrix4 &m, const Matrix4 &n) {
    pos = m * pos;
    normal = n * normal;
}

Vertex Vertex::getTransform(const Matrix4 &m, const Matrix4 &n) {
    auto v = *this;
    v.transform(m, n);
    return v;
}

Vertex Vertex::interpolate(Vertex &other, float t) const {
    return { pos.lerp(other.pos, t), normal.lerp(other.normal, t), lerpf(material, other.material, t) };
}

Vertex operator*(const Matrix4 &m, Vertex v) {
    v.transform(m);
    return v;
}
