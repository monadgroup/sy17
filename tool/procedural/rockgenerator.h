#pragma once
#include "def.h"

namespace monad {

    class RawObject;
    class Context;
    class Program;
    class RawObject;
    class FeedbackObject;
    template<class T> class Uniform;

    class RockGenerator {
        Context *context;

        RawObject *_rockPatch;
        SHARED_PTR(Program) _rockProgram;
        Uniform<float> *_seedUniform;

    public:
        explicit RockGenerator(Context *context);

        FeedbackObject *generate();
    };

}
