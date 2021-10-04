#include "program.h"
#include "shader.h"
#include "debug.h"
#include "renderer/texture.h"
#include "renderer/uniformbuffer.h"
#include "renderer/framebuffer.h"

using namespace monad;

Program::Program(GLuint resource)
        : resource(resource) {

}

void Program::attachShader(std::shared_ptr<Shader> shader) {
    if (completed) return; // no!
    glAttachShader(resource, shader->resource);
    shaders.push_back(shader);
    shader->programs.push_back(shared_from_this());
}

void Program::complete() {
    completed = true;
    link();
}

void Program::link() {
    glLinkProgram(resource);

    auto linkOk = GL_FALSE;
    glGetProgramiv(resource, GL_LINK_STATUS, &linkOk);
    if (!linkOk) {
        auto maxLength = 0;
        glGetProgramiv(resource, GL_INFO_LOG_LENGTH, &maxLength);
        auto logval = new char[maxLength];
        glGetProgramInfoLog(resource, maxLength, &maxLength, logval);

        auto linkStr = std::string("Issue linking ");
        for (auto i = 0; i < shaders.size(); i++) {
            linkStr.append(shaders[i]->path);
            if (i == shaders.size() - 2) linkStr.append(" and ");
            else if (i != shaders.size() - 1) linkStr.append(", ");
        }

        log(linkStr.c_str());
        log(logval);
        delete[] logval;
    }

    // update locations
    for (auto &entry : locatables) {
        entry.second->updateLocation(entry.first.c_str());
    }
}

void Program::addLocatable(const char *name, Locatable *dat) {
    locatables.insert(std::pair<std::string, Locatable*>(name, dat));
}

Uniform<int> *Program::setSampler(const char *name, const Texture *tex) {
    return getUniform<int>(name, tex->unit);
}

UniformBuffer* Program::setBuffer(const char *name, GLuint loc) {
    return getUniformBuffer(name, loc);
}

UniformBuffer* Program::getUniformBuffer(const char *name, GLuint def) {
    auto rname = std::string(name);
    auto entry = locatables.find(rname);

    if (entry == locatables.end()) {
        auto item = new UniformBuffer(def, resource, name);
        addLocatable(name, item);
        return (UniformBuffer*)locatables.find(rname)->second;
    }

    return (UniformBuffer*)entry->second;
}
