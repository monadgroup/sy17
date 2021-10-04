#pragma once
#include "vector.h"

namespace monad {

    class Framebuffer;
    class Texture;
    class Context;
    template<class T> class Uniform;

    class PingPong {
        Framebuffer *initialFbA;
        Framebuffer *initialFbB;
        Texture *initialTexA;
        Texture *initialTexB;

        Framebuffer *fbA;
        Framebuffer *fbB;
        Texture *texA;
        Texture *texB;

    public:
        PingPong(Context *context, Texture *texA, Texture *texB);
        ~PingPong();

        void swap();
        void reset();

        Texture *getReadTex() { return texA; }
        Texture *getWriteTex() { return texB; }
        Texture *getBaseReadTex() { return initialTexA; }
        Texture *getBaseWriteTex() { return initialTexB; }

        Framebuffer *getReadFramebuffer() { return fbA; }
        Framebuffer *getWriteFramebuffer() { return fbB; }
        Framebuffer *getBaseReadFramebuffer() { return initialFbA; }
        Framebuffer *getBaseWriteFramebuffer() { return initialFbB; }

        void bind(const ivec4 &viewport, Uniform<int> *sampler);
        void swapAndBind(const ivec4 &viewport, Uniform<int> *sampler);
        void resetAndBind(const ivec4 &viewport, Uniform<int> *sampler);
    };

}
