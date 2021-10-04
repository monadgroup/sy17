#pragma once
#include "def.h"
#include "vector.h"
#include "list.h"
#include "bloomblurunit.h"

namespace monad {

    class Context;
    class Texture;
    class Program;
    class Framebuffer;
    template<class T> class Uniform;
    template<class T> class Track;
    class PingPong;
    class Timer;

    class BloomUnit {
        Context *context;
        Timer *timer;
        static constexpr int unitCount = 4;

        Texture *inHighlights;

        SHARED_PTR(Program) copyProgram;
        Uniform<int> *copySampler;

        SHARED_PTR(Program) program;
        Uniform<int> *colorSampler;
        Uniform<vec2> *direction;
        Uniform<vec2> *viewportSize;

        SHARED_PTR(Program) compositeProgram;
        Framebuffer *compositeFb;
        Uniform<float> *bloomBias;
        Uniform<float> *bloomPower;
        Uniform<float> *bloomAmount;

        BloomBlurUnit *units[unitCount];

        Track<float> *bloomBiasTrack;
        Track<float> *bloomPowerTrack;
        Track<float> *bloomAmountTrack;

        Texture *outColor;

    public:
        explicit BloomUnit(Context *context, int iterationCount = 1);

        void setInputs(Texture *highlights);
        void render();

        Texture *getColorTexture() { return outColor; }
    };

}
