#include "generators.h"
#include "mesh.h"
#include "polygon.h"

using namespace gen;
using namespace monad;

static void addQuad(List<Polygon> &polygonList, const Vertex &p0, const Vertex &p1, const Vertex &p2, const Vertex &p3) {
    Vertex poly1Points[3] = {p0, p1, p2};
    polygonList.push_back(Polygon(poly1Points));
    Vertex poly2Points[3] = {p2, p3, p0};
    polygonList.push_back(Polygon(poly2Points));
}

void gen::quad(Mesh *mesh, float material, const vec2 &size, const Matrix4 &transform) {
    auto normalTransform = transform.invert().transpose();
    vec3 normal = normalTransform * vec3(0, 1, 0);
    vec3 size3 = { size.x, 0, size.y };

    auto p0 = Vertex(transform * (vec3(-1, 0, -1) * size3), normal, material);
    auto p1 = Vertex(transform * (vec3(1, 0, -1) * size3), normal, material);
    auto p2 = Vertex(transform * (vec3(1, 0, 1) * size3), normal, material);
    auto p3 = Vertex(transform * (vec3(-1, 0, 1) * size3), normal, material);

    mesh->faces.reserve(mesh->faces.size() + 2);
    addQuad(mesh->faces, p0, p1, p2, p3);
}

void gen::box(Mesh *mesh, float material, const vec3 &size, const Matrix4 &transform) {
    mesh->faces.reserve(mesh->faces.size() + 12);

    vec3 faceNormals[6] = { {-1, 0, 0},
                            {0, 1, 0},
                            {1, 0, 0},
                            {0, -1, 0},
                            {0, 0, -1},
                            {0, 0, 1} };
    int faces[6][4] = { {0, 1, 2, 3},
                        {3, 2, 6, 7},
                        {7, 6, 5, 4},
                        {4, 5, 1, 0},
                        {5, 6, 2, 1},
                        {7, 4, 0, 3}};

    vec3 v[8];
    v[0].x = v[1].x = v[2].x = v[3].x = -1;
    v[4].x = v[5].x = v[6].x = v[7].x = 1;
    v[0].y = v[1].y = v[4].y = v[5].y = -1;
    v[2].y = v[3].y = v[6].y = v[7].y = 1;
    v[0].z = v[3].z = v[4].z = v[7].z = 1;
    v[1].z = v[2].z = v[5].z = v[6].z = -1;

    auto normalTransform = transform.invert().transpose();

    for (auto i = 0; i < 6; i++) {
        auto n = normalTransform * faceNormals[i];

        auto p0 = Vertex(transform * (v[faces[i][0]] * size), n, material);
        auto p1 = Vertex(transform * (v[faces[i][1]] * size), n, material);
        auto p2 = Vertex(transform * (v[faces[i][2]] * size), n, material);
        auto p3 = Vertex(transform * (v[faces[i][3]] * size), n, material);
        addQuad(mesh->faces, p0, p1, p2, p3);
    }
}

void gen::cylinder(Mesh *mesh, float material, float height, vec2 radii, const Matrix4 &transform, int slices) {
    float sliceAngle = 2 * (float)M_PI / slices;

    auto topVertices = List<Vertex>(slices);
    auto bottomVertices = List<Vertex>(slices);
    mesh->faces.reserve(mesh->faces.size() + slices * 4);

    float extra = radii.y - radii.x;
    float h = sqrtf(extra*extra + height*height);
    float normalY = extra / h;
    float horizontalAmt = height / h;

    auto normalTransform = transform.invert().transpose();

    for (auto i = 0; i < slices; i++) {
        float angle = sliceAngle * i;
        vec2 baseCoord = { cosf(angle), sinf(angle) };

        vec3 n = normalTransform * vec3(horizontalAmt * baseCoord.x, normalY, horizontalAmt * baseCoord.y);
        vec3 topP = transform * vec3(baseCoord.x * radii.x, height / 2, baseCoord.y * radii.x);
        vec3 bottomP = transform * vec3(baseCoord.x * radii.y, -height / 2, baseCoord.y * radii.y);

        topVertices.push_back(Vertex(topP, n, material));
        bottomVertices.push_back(Vertex(bottomP, n, material));
    }

    // generate polygons from slice vertices
    for (auto i = 0; i < slices; i++) {
        auto topVertex1 = topVertices[i];
        auto topVertex2 = topVertices[(i + 1) % slices];
        auto bottomVertex1 = bottomVertices[i];
        auto bottomVertex2 = bottomVertices[(i + 1) % slices];

        Vertex vertices1[] = { topVertex1, bottomVertex1, bottomVertex2 };
        Vertex vertices2[] = { bottomVertex2, topVertex2, topVertex1 };

        mesh->faces.push_back(Polygon(vertices1));
        mesh->faces.push_back(Polygon(vertices2));
    }

    // generate top and bottom face polygons
    auto topN = normalTransform * vec3(0, 1, 0);
    auto topCenterP = transform * vec3(0, height / 2, 0);
    auto topCenterVertex = Vertex(topCenterP, topN, material);

    auto bottomN = normalTransform * vec3(0, -1, 0);
    auto bottomCenterP = transform * vec3(0, -height / 2, 0);
    auto bottomCenterVertex = Vertex(bottomCenterP, bottomN, material);

    for (auto i = 0; i < slices; i++) {
        auto topVertex1 = Vertex(topVertices[i].pos, topN, material);
        auto topVertex2 = Vertex(topVertices[(i + 1) % slices].pos, topN, material);
        Vertex vertices1[] = { topVertex1, topVertex2, topCenterVertex };
        mesh->faces.push_back(Polygon(vertices1));

        auto bottomVertex1 = Vertex(bottomVertices[i].pos, bottomN, material);
        auto bottomVertex2 = Vertex(bottomVertices[(i + 1) % slices].pos, bottomN, material);
        Vertex vertices2[] = { bottomVertex2, bottomVertex1, bottomCenterVertex };
        mesh->faces.push_back(Polygon(vertices2));
    }
}
