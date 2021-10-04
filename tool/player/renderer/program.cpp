#include "program.h"
#include "shader.h"
#include "renderer/texture.h"
#include "renderer/uniformbuffer.h"

using namespace monad;

Program::Program(GLuint resource) : resource(resource) { }

void Program::attachShader(Shader *shader) {
    glAttachShader(resource, shader->resource);
}

void Program::complete() {
    link();
}

void Program::link() {
    glLinkProgram(resource);
}

Uniform<int> * Program::setSampler(const char *name, const Texture *tex) {
    return getUniform<int>(name, tex->unit);
}

UniformBuffer* Program::setBuffer(const char *name, GLuint loc) {
    return getUniformBuffer(name, loc);
}

UniformBuffer* Program::getUniformBuffer(const char *name, GLuint def) {
    return new UniformBuffer(def, resource, name);
}
