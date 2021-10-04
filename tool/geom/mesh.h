#pragma once
#include "list.h"
#include "vector.h"
#include "polygon.h"

namespace monad {

    class Mesh {
    public:
        List<Polygon> faces;

        Mesh() = default;
        explicit Mesh(const List<Polygon> &faces);

        void place(const Mesh *mesh, Matrix4 transform = Matrix4::getIdentity());
    };

}
