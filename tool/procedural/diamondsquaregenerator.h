#pragma once
#include "def.h"
#include "vector.h"

namespace monad {

    class Texture;

    class DiamondSquareGenerator {
    public:
        Texture *generate(ivec2 size, int featureSize);
    };

}
