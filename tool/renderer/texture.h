#pragma once

#include "gl.h"
#include "vector.h"

namespace monad {

    class Texture;

    struct TextureImage {
        bool isNone;
        Texture *texture;
        GLint level;

        TextureImage();
        TextureImage(Texture *texture, GLint level);
    };

    class Texture {
    public:
        static GLuint nextTextureUnit;

        GLenum internalFormat, format, type;

        GLuint resource;
        GLuint unit;
        GLint levelCount = 0;

        ivec2 size;

        explicit Texture(const char *debugName, GLenum internalFormat = GL_RGBA32F, GLenum format = GL_RGBA, GLenum type = GL_FLOAT,
                         GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE,
                         GLenum magFilter = GL_LINEAR, GLenum minFilter = GL_LINEAR, GLenum compareMode = GL_NONE,
                         GLenum compareFunc = GL_LESS);
        ~Texture();

        void set(GLenum prop, GLenum val);
        void beginSet();
        void doSet(GLenum prop, GLenum val);
        void finishSet();

        TextureImage addImage(ivec2 size);
        TextureImage setImage(GLint level, ivec2 size, void *data = nullptr);
        TextureImage getImage(GLint level);
    };

}
