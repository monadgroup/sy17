#include "diamondsquaregenerator.h"
#include "renderer/context.h"
#include "renderer/program.h"
#include "renderer/pingpong.h"
#include "util.h"
#include "debug.h"

using namespace monad;

static float signedRand() {
    return random(-1, 1);
}

static float *getCell(float *buffer, int bufferWidth, int bufferHeight, int x, int y) {
    return buffer + ((x & (bufferWidth - 1)) + (y & (bufferHeight - 1)) * bufferWidth);
}

static void sampleSquare(float *buffer, int bufferWidth, int bufferHeight, int x, int y, int size,
                         float value) {
    int hs = size / 2;
    float a = *getCell(buffer, bufferWidth, bufferHeight, x - hs, y - hs);
    float b = *getCell(buffer, bufferWidth, bufferHeight, x + hs, y - hs);
    float c = *getCell(buffer, bufferWidth, bufferHeight, x - hs, y + hs);
    float d = *getCell(buffer, bufferWidth, bufferHeight, x + hs, y + hs);

    *getCell(buffer, bufferWidth, bufferHeight, x, y) = (a + b + c + d) / 4 + value;
}

static void sampleDiamond(float *buffer, int bufferWidth, int bufferHeight, int x, int y, int size,
                          float value) {
    int hs = size / 2;
    float a = *getCell(buffer, bufferWidth, bufferHeight, x - hs, y);
    float b = *getCell(buffer, bufferWidth, bufferHeight, x + hs, y);
    float c = *getCell(buffer, bufferWidth, bufferHeight, x, y - hs);
    float d = *getCell(buffer, bufferWidth, bufferHeight, x, y + hs);

    *getCell(buffer, bufferWidth, bufferHeight, x, y) = (a + b + c + d) / 4 + value;
}

static void diamondSquare(float *buffer, int bufferWidth, int bufferHeight, int stepSize,
                          float scale) {
    int halfStep = stepSize / 2;

    for (int y = halfStep; y < bufferHeight + halfStep; y += stepSize) {
        for (int x = halfStep; x < bufferWidth + halfStep; x += stepSize) {
            sampleSquare(buffer, bufferWidth, bufferHeight, x, y, stepSize,
                       signedRand() * scale);
        }
    }

    for (int y = 0; y < bufferHeight; y += stepSize) {
        for (int x = 0; x < bufferWidth; x += stepSize) {
            sampleDiamond(buffer, bufferWidth, bufferHeight, x + halfStep, y, stepSize, signedRand() * scale);
            sampleDiamond(buffer, bufferWidth, bufferHeight, x, y + halfStep, stepSize, signedRand() * scale);
        }
    }
}

Texture* DiamondSquareGenerator::generate(ivec2 size, int featureSize) {
    dbgEnter("Generate diamond-square");
    auto bufferSize = size.x * size.y;
    auto buffer = new float[bufferSize];

    for (auto i = 0; i < bufferSize; i++) {
        buffer[i] = signedRand();
    }

    auto scale = 1.0f;
    while (featureSize > 1) {
        diamondSquare(buffer, size.x, size.y, featureSize, scale);
        featureSize /= 2;
        scale /= 2;
    }

    auto tex = new Texture("DiamondSquare", GL_R32F, GL_RED, GL_FLOAT, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_NONE);
    tex->setImage(0, size, buffer);
    dbgExit();
    return tex;
}
