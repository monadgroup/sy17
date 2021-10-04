#pragma once
#include "dependency/shaderdependency.h"
#include <memory>
#include <vector>
#include <string>
#include <GL/gl.h>
#include "../../../glslang/glslang/Public/ShaderLang.h"

namespace monad {

    class Program;

    class Shader : public monad::ShaderDependency {
    public:
        GLuint resource;
        ShHandle handle;
        std::vector<std::weak_ptr<Program>> programs;

        explicit Shader(GLuint resource, GLenum type, const std::string &path);
        ~Shader() override;

        void build() override;
    };

}
