#pragma once
#include "renderer/uniform.h"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include "renderer/gl.h"
#include "renderer/locatable.h"

namespace monad {

    class Shader;
    class Texture;
    class UniformBuffer;
    class IFramebuffer;

    class Program : public std::enable_shared_from_this<Program> {
        bool completed = false;

        std::vector<std::shared_ptr<Shader>> shaders;
        std::map<std::string, Locatable*> locatables;

        void addLocatable(const char *name, Locatable *dat);

    public:
        GLuint resource;

        explicit Program(GLuint resource);

        void attachShader(std::shared_ptr<Shader> shader);
        void complete();
        void link();

        Uniform<int> *setSampler(const char *name, const Texture *tex);
        UniformBuffer *setBuffer(const char *name, GLuint loc);

        template<class T>
        Uniform<T> *getUniform(const char *name, const T &def) {
            auto rname = std::string(name);
            auto entry = locatables.find(rname);

            if (entry == locatables.end()) {
                auto uniform = new Uniform<T>(def, resource, name);
                addLocatable(name, uniform);
                return (Uniform<T>*)locatables.find(rname)->second;
            }

            return (Uniform<T>*)entry->second;
        }

        UniformBuffer *getUniformBuffer(const char *name, GLuint def);
    };

}
