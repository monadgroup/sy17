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
    class Timer;

    class DofUnit {
        Context *context;
        Timer *timer;

        Texture *inColor = nullptr;
        Texture *inDepth = nullptr;

        SHARED_PTR(Program) program;
        Framebuffer *fb;

        Uniform<int> *colorSampler;
        Uniform<int> *depthSampler;
        Uniform<vec2> *focusRange;
        Uniform<float> *focusGradient;
        Uniform<float> *focusSize;

        Track<vec2> *focusRangeTrack;
        Track<float> *focusGradientTrack;
        Track<float> *focusSizeTrack;

        Texture *outColor;

    public:
        explicit DofUnit(Context *context);

        void setInputs(Texture *color, Texture *depth);
        void render();

        Texture *getColorTexture() { return outColor; }
    };

}
