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

    class FlareUnit {
        Context *context;
        Timer *timer;
        ivec2 size;

        Texture *inColor = nullptr;

        SHARED_PTR(Program) downsampleProgram;
        SHARED_PTR(Program) effectProgram;
        SHARED_PTR(Program) blurProgram;

        Framebuffer *downsampleFb;
        PingPong *pingPong;

        Texture *downsampled;

        Uniform<int> *colorSampler;
        Uniform<float> *scale;
        Uniform<float> *bias;

        Uniform<int> *ghostCount;
        Uniform<float> *ghostDispersal;
        Uniform<float> *haloWidth;

        Uniform<int> *blurSampler;
        Uniform<vec2> *blurDirection;

        Track<float> *scaleTrack;
        Track<float> *biasTrack;
        Track<int> *ghostCountTrack;
        Track<float> *ghostDispersalTrack;
        Track<float> *haloWidthTrack;

    public:
        explicit FlareUnit(Context *context);

        void setInputs(Texture *color);
        void render();

        Texture *getColorTexture() const;
    };

}
