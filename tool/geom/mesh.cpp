#include "mesh.h"
#include "polygon.h"

using namespace monad;

Mesh::Mesh(const List<Polygon> &faces) : faces(faces) { }

void Mesh::place(const Mesh *mesh, Matrix4 transform) {
    auto normalTransform = transform.invert().transpose();

    faces.reserve(faces.size() + mesh->faces.size());
    for (const auto &face : mesh->faces) {
        faces.push_back(face.getTransform(transform, normalTransform));
    }
}
