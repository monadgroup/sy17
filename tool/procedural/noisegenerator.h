#pragma once
#include "vector.h"

namespace monad {

    class Texture;

    class NoiseGenerator {
    public:
        Texture *grayscale1024;

        NoiseGenerator();

        Texture *getGrayscale(ivec2 size);
    };

}
