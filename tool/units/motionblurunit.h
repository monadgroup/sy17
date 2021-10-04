#pragma once
#include "def.h"

namespace monad {

    class Context;
    class Texture;
    class Program;
    class Framebuffer;
    template<class T> class Uniform;
    class Timer;

    class MotionBlurUnit {
        Context *context;
        Timer *timer;

        Texture *inColor = nullptr;
        Texture *inDepth = nullptr;
        Texture *inVelocity = nullptr;

        SHARED_PTR(Program) program;
        Framebuffer *fb;

        Uniform<int> *colorSampler;
        Uniform<int> *depthSampler;
        Uniform<int> *velocitySampler;

        Texture *outColor;

    public:
        explicit MotionBlurUnit(Context *context);

        void setInputs(Texture *color, Texture *depth, Texture *velocity);
        void render();

        Texture *getColorTexture() { return outColor; }
    };

}
