#pragma once
#include "gl.h"

namespace monad {

    class Ubo {
        GLuint binding;

    public:
        GLenum mode;
        GLuint resource;

        explicit Ubo(GLenum mode);

        void setData(GLsizeiptr size, const GLvoid *data);
        float *mapData(GLsizeiptr size);
        void unmapData();

        void setBinding(GLuint binding);
        GLuint getBinding() { return binding; }
    };

}
