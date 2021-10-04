#pragma once
#include "def.h"
#include "vector.h"
#include "list.h"
#include "../geom/mesh.h"

namespace monad {

    class Mesh;

    class ObeliskGenerator {
        List<Mesh> sides;
        vec3 realSize;
        float cellSize;
        float insideMat;

        void generateFace(Mesh *mesh, const ivec2 &size, float cellSize, float height, float mat, const Matrix4 &transform);

    public:
        ObeliskGenerator(int sideVariations, ivec3 size, float cellSize, float insideMat, float outsideMat);

        void generate(Mesh *mesh, const Matrix4 &transform, bool inside = true);
    };

}
