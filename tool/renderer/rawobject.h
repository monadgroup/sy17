#pragma once
#include "gl.h"
#include "def.h"

namespace monad {

    class Program;

    class RawObject {
        GLuint _attribCount = 0;
        GLint _vertexCount = 0;

    public:
        GLuint buffer;
        GLuint vao;
        GLenum usageMode;
        GLenum renderMode;

        RawObject(GLuint buffer, GLuint vao, GLenum usageMode = GL_STATIC_DRAW, GLenum renderMode = GL_PATCHES);

        virtual void setData(int vertexCount, int dataSize, const float *data);
        virtual float *mapData(int vertexCount, int dataSize);
        virtual void unmapData();

        virtual void attrib(GLint size, GLsizei stride, GLint pointer);
        virtual void startAttrib();
        virtual void doAttrib(GLint size, GLsizei stride, GLint pointer);
        virtual void endAttrib();

        virtual void render(const SHARED_PTR(Program) program, int instanceCount = 1);
        virtual void startRender(const SHARED_PTR(Program) program);
        virtual void doRender(int instanceCount = 1);
        virtual void endRender();
    };

}
