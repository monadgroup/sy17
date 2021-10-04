#pragma once
#include <vector>
#include <string>
#include <memory>
#include <GL/gl.h>

namespace monad {

    struct ShaderKey {
        std::string name;
        GLenum type;
    };
    bool operator<(const ShaderKey &a, const ShaderKey &b);

    extern std::vector<ShaderKey> shaderJournal;
    extern std::vector<std::string> trackJournal;

}
