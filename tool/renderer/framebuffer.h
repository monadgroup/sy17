#pragma once
#include <GL/gl.h>
#include "vector.h"
#include "list.h"

namespace monad {

    class TextureImage;

    class IFramebuffer {
    public:
        vec4 clearColor = {0, 0, 0, 0};
        float clearDepth = 1;
        GLuint resource;

        explicit IFramebuffer(GLuint resource);
        void bind(ivec4 viewport, bool clear = true) const;
        void bind(int viewportCount, const ivec4 *viewports, bool clear = true) const;
    };

    class ScreenFramebuffer: public IFramebuffer {
    public:
        explicit ScreenFramebuffer();
    };

    class Framebuffer : public IFramebuffer {
    public:
        TextureImage *colors;
        TextureImage *depth = nullptr;
        TextureImage *stencil = nullptr;

        Framebuffer(GLuint resource, int colorCount, const TextureImage *colors, const TextureImage *depth = nullptr, const TextureImage *stencil = nullptr);
    };

}
