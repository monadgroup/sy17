#pragma once
#include "def.h"
#include "list.h"

namespace monad {

    class Texture;
    class Context;
    class Program;
    class Framebuffer;
    template<class T> class Uniform;
    template<class T> class Track;
    class Timer;

    class CompositeUnit {
        Context *context;
        Timer *timer;

        Texture *inColor;
        Texture *inBloom;
        Texture *inFlare;

        SHARED_PTR(Program) program;
        Framebuffer *fb;

        Uniform<int> *colorSampler;
        Uniform<int> *bloomSampler;
        Uniform<int> *flareSampler;
        Uniform<float> *exposure;
        Uniform<float> *vignetteStrength;
        Uniform<float> *vignetteSize;
        Uniform<float> *vignettePower;
        Uniform<float> *grainAmount;

        Track<float> *exposureTrack;
        Track<float> *vignetteStrengthTrack;
        Track<float> *vignetteSizeTrack;
        Track<float> *vignettePowerTrack;
        Track<float> *grainAmountTrack;

        Texture *outColor;

    public:
        explicit CompositeUnit(Context *context);

        void setInputs(Texture *color, Texture *bloom, Texture *flare);
        void render();

        Texture *getColorTexture() { return outColor; }
    };

}
