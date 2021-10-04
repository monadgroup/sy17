#include "obeliskgenerator.h"
#include "../geom/mesh.h"
#include "../geom/generators.h"
#include "shaders/common/materials.glsl"
#include "distributedsquaregenerator.h"
#include "util.h"

using namespace monad;

void ObeliskGenerator::generateFace(Mesh *mesh, const ivec2 &size, float cellSize, float height, float mat, const Matrix4 &transform) {
    auto squareGen = DistributedSquareGenerator(size);

    auto boxes = List<ivec4>();
    squareGen.generate(boxes);

    for (const auto &box : boxes) {
        auto topLeft = ivec2(box.x, box.y);
        auto boxSize = ivec2(box.z, box.w);
        auto bottomRight = topLeft + boxSize;

        auto centerP = vec2(topLeft + bottomRight)/2 * cellSize;
        auto realSize = vec2(boxSize)/2 * cellSize;

        gen::box(mesh, mat - 0.5f, {realSize.x, height, realSize.y}, transform * Matrix4::move({centerP.x, -height, centerP.y}));
    }
}

ObeliskGenerator::ObeliskGenerator(int sideVariations, ivec3 size, float cellSize, float insideMat, float outsideMat) : sides(sideVariations), cellSize(cellSize), insideMat(insideMat) {
    realSize = vec3(size.x, size.y, size.x) * cellSize;

    auto sideTransform = Matrix4::move({cellSize*4 - realSize.x/2, 0, realSize.x/2 - cellSize*2}) * Matrix4::rotateX(M_PI/2);
    for (auto i = 0; i < sideVariations; i++) {
        auto mesh = Mesh();
        generateFace(&mesh, {size.x-8, size.y-8}, cellSize, cellSize, outsideMat, sideTransform);
        sides.push_back(mesh);
    }
}

void ObeliskGenerator::generate(Mesh *mesh, const Matrix4 &transform, bool inside) {
    if (inside) {
        gen::box(mesh, insideMat - 0.5f, realSize / 2 - vec3(cellSize, cellSize, cellSize) * 2,
                 transform * Matrix4::move({0, realSize.y / 2 - cellSize * 4, 0}));
    }

    for (auto i = 0; i < 4; i++) {
        auto pickSide = &sides[(int)random(0, sides.size())];
        mesh->place(pickSide, transform * Matrix4::rotateY(M_PI/2*i));
    }
}
