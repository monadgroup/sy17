#pragma once
#include "bloomunit.h"
#include "dofunit.h"
#include "motionblurunit.h"
#include "flareunit.h"
#include "compositeunit.h"
#include "fxaaunit.h"

namespace monad {

    class Texture;
    class Context;

    class PostUnit {
        Context *context;

        Texture *inPos = nullptr;
        Texture *inNormal = nullptr;
        Texture *inDepth = nullptr;

        BloomUnit bloom;
        DofUnit dof;
        MotionBlurUnit motionBlur;
        FlareUnit flare;
        CompositeUnit composite;
        FxaaUnit fxaa;

    public:
        explicit PostUnit(Context *context);

        void setInputs(Texture *color, Texture *depth, Texture *velocity, Texture *highlights);
        void render();
    };

}
