#include "shader.h"
#include "renderer/gl.h"

using namespace monad;

Shader::Shader(GLuint resource, int fragmentCount, const char **fragments) : resource(resource) {
    glShaderSource(resource, fragmentCount, fragments, nullptr);
    glCompileShader(resource);
}
