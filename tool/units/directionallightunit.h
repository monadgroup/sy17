#pragma once
#include "def.h"
#include "vector.h"
#include "irenderable.h"

namespace monad {

    class Texture;
    class Context;
    class Program;
    class Framebuffer;
    template<class T> class Uniform;
    class PingPong;
    class DirectionalLight;
    class Timer;

    class DirectionalLightUnit {
        Context *context;
        Timer *timer;
        ivec2 resolution;

        SHARED_PTR(DirectionalLight) light;

        SHARED_PTR(Program) program;
        PingPong *pingPong;
        Framebuffer *renderFb;

        Uniform<int> *colorSampler;
        Uniform<vec2> *blurDirection;

    public:
        explicit DirectionalLightUnit(Context *context, ivec2 resolution);

        void setLight(SHARED_PTR(DirectionalLight) light);
        SHARED_PTR(DirectionalLight) getLight() { return light; }
        void render(IRenderable *renderable);

        Texture *getVarianceTexture();
    };

}

