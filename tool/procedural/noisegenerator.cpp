#include "noisegenerator.h"
#include "renderer/context.h"
#include "debug.h"

using namespace monad;

NoiseGenerator::NoiseGenerator() {
    grayscale1024 = getGrayscale({1024, 1024});
}

Texture* NoiseGenerator::getGrayscale(ivec2 size) {
    dbgEnter("Generate grayscale noise");
    auto bufferSize = size.x * size.y;
    auto buffer = new float[bufferSize];
    for (auto i = 0; i < bufferSize; i++) {
        auto r = rand();
        //printf("getGrayscale=%i\n", r);
        buffer[i] = r / (float)RAND_MAX;
    }

    auto tex = new Texture("Noise", GL_R32F, GL_RED, GL_FLOAT, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_NONE);
    tex->setImage(0, size, buffer);
    dbgExit();
    return tex;
}

