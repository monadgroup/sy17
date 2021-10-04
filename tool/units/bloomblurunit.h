#pragma once
#include "def.h"
#include "vector.h"
#include <GL/gl.h>

namespace monad {

    class Context;
    class Texture;
    class Program;
    class Framebuffer;
    template<class T> class Uniform;
    template<class T> class Track;
    class PingPong;

    class BloomBlurUnit {
        Context *context;
        int iterationCount;

        ivec2 viewport;

        PingPong *pingPong;

    public:
        BloomBlurUnit(Context *context, ivec2 viewport, int iterationCount, GLenum internalFormat, GLenum format);

        void bindSourceFramebuffer();
        void render(const SHARED_PTR(Program) program, Uniform<int> *sampler, Uniform<vec2> *direction);

        ivec2 getViewport() const { return viewport; }
        Texture *getColorTexture() const;
    };

}

