#pragma once
#include "def.h"

namespace monad {

    class Texture;
    class Context;
    class Program;
    template<class T> class Uniform;
    class Timer;

    class FxaaUnit {
        Context *context;
        Timer *timer;

        Texture *inColor = nullptr;

        SHARED_PTR(Program) program;

        Uniform<int> *colorSampler;

    public:
        explicit FxaaUnit(Context *context);

        void setInputs(Texture *color);
        void render();
    };

}
