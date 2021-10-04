#pragma once
#include "rawobject.h"
#include "ubo.h"
#include "vector.h"

namespace monad {

    class Mesh;
    class Vertex;

    class Object : protected RawObject {
        Matrix4 transform = Matrix4::getIdentity();

    public:
        Ubo ubo;
        Mesh *mesh;

        Object(GLuint buffer, GLuint vao, Mesh *mesh, GLenum usageMode, GLenum renderMode);

        void update();
        void render(const SHARED_PTR(Program) program, int instanceCount) override;
        void startRender(const SHARED_PTR(Program) program) override;
        void doRender(int instanceCount) override;
        void endRender() override;

        void setTransform(const Matrix4 &transform);

        static constexpr int strideCount = 7;
        static void placeVertex(float *buffer, const Vertex &vertex);
    };

}
