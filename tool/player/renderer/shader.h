#pragma once
#include <GL/gl.h>

namespace monad {

    class Program;

    class Shader {
    public:
        GLuint resource;

        explicit Shader(GLuint resource, int fragmentCount, const char **fragments);
    };

}
