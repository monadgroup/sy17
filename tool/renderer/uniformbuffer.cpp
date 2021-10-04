#include "uniformbuffer.h"
#include "gl.h"

using namespace monad;

UniformBuffer::UniformBuffer(const GLuint &def, GLuint program, const char *name)
    : LocatableUniform(def, program) {
    updateLocation(name);
}

void UniformBuffer::updateLocation(const char *name) {
    resource = glGetUniformBlockIndex(program, name);
    updateVal();
}

void UniformBuffer::updateVal() {
    glUniformBlockBinding(program, resource, val);
}
