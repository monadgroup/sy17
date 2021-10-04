#pragma once
#include "def.h"
#include "vector.h"

namespace monad {

    class Context;
    class Texture;
    class Program;
    class Framebuffer;
    template<class T> class Uniform;
    template<class T> class Track;
    class PingPong;
    class Timer;

    class SsaoUnit {
        Context *context;
        Timer *timer;

        Texture *inPos = nullptr;
        Texture *inDepth = nullptr;
        Texture *inNormal = nullptr;

        SHARED_PTR(Program) genProgram;
        SHARED_PTR(Program) blurProgram;

        Framebuffer *genFb;
        Framebuffer *blurFb;

        Uniform<int> *posSampler;
        Uniform<int> *depthSampler;
        Uniform<int> *normalSampler;
        Uniform<float> *radius;
        Uniform<float> *power;

        Track<float> *radiusTrack;
        Track<float> *powerTrack;

        Texture *outColor;

    public:
        explicit SsaoUnit(Context *context);

        void setInputs(Texture *pos, Texture *depth, Texture *normal);
        void render();

        Texture *getColorTexture() { return outColor; }
    };

}
