#include "object.h"
#include "debug.h"
#include "renderer/program.h"
#include "geom/mesh.h"
#include "geom/polygon.h"
#include "defines.h"

using namespace monad;

Object::Object(GLuint buffer, GLuint vao, Mesh *mesh, GLenum usageMode, GLenum renderMode)
    : RawObject(buffer, vao, usageMode, renderMode), mesh(mesh), ubo(GL_DYNAMIC_DRAW) {
    update();
    setTransform(Matrix4::getIdentity());
}

void Object::update() {
    dbgEnter("Update object");
    auto verticesPerItem = 3;
    auto polyCount = mesh->faces.size();
    auto vertexCount = polyCount * verticesPerItem;
    auto bufferSize = vertexCount * strideCount * sizeof(float);
    auto bufferData = mapData(vertexCount, bufferSize);
    for (size_t i = 0; i < polyCount; i++) {
        auto placePos = bufferData + i * strideCount * verticesPerItem;
        auto polygon = &mesh->faces[i];

        for (auto j = 0; j < verticesPerItem; j++) {
            placeVertex(placePos + strideCount * j, polygon->vertices[j]);
        }
    }
    unmapData();
    dbgExit();
}

void Object::render(const SHARED_PTR(Program) program, int instanceCount) {
    startRender(program);
    doRender(instanceCount);
    endRender();
}

void Object::startRender(const SHARED_PTR(Program) program) {
    RawObject::startRender(program);
    glBindVertexBuffer(0, buffer, 0, strideCount * sizeof(float));
    ubo.setBinding(OBJECT_BUFFER_BINDING);
}

void Object::doRender(int instanceCount) {
    RawObject::doRender(instanceCount);
}

void Object::endRender() {
    glBindVertexBuffer(0, 0, 0, 0);
    RawObject::endRender();
}

void Object::setTransform(const Matrix4 &transform) {
    dbgEnter("Set object transform");
    ubo.setData(16 * sizeof(float), transform.transpose().m);
    this->transform = transform;
    dbgExit();
}

void Object::placeVertex(float *buffer, const Vertex &vertex) {
    buffer[0] = vertex.pos.x;
    buffer[1] = vertex.pos.y;
    buffer[2] = vertex.pos.z;
    buffer[3] = vertex.normal.x;
    buffer[4] = vertex.normal.y;
    buffer[5] = vertex.normal.z;
    buffer[6] = vertex.material;
}
