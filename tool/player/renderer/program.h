#pragma once
#include "renderer/gl.h"
#include "renderer/uniform.h"

namespace monad {

    class Shader;
    class Texture;
    class UniformBuffer;

    class Program {
    public:
        GLuint resource;

        explicit Program(GLuint resource);

        void attachShader(Shader *shader);
        void complete();
        void link();

        Uniform<int> *setSampler(const char *name, const Texture *tex);
        UniformBuffer *setBuffer(const char *name, GLuint loc);

        template<class T>
        Uniform<T> *getUniform(const char *name, const T &def) {
            return new Uniform<T>(def, resource, name);
        }

        UniformBuffer *getUniformBuffer(const char *name, GLuint def);
    };

}
