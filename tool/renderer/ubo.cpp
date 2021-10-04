#include "ubo.h"

using namespace monad;

Ubo::Ubo(GLenum mode) : mode(mode) {
    glGenBuffers(1, &resource);
}

void Ubo::setData(GLsizeiptr size, const GLvoid *data) {
    glBindBuffer(GL_UNIFORM_BUFFER, resource);
    glBufferData(GL_UNIFORM_BUFFER, size, data, mode);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

float* Ubo::mapData(GLsizeiptr size) {
    glBindBuffer(GL_UNIFORM_BUFFER, resource);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, mode);
    return (float*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
}

void Ubo::unmapData() {
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void Ubo::setBinding(GLuint binding) {
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, resource);
    this->binding = binding;
}
