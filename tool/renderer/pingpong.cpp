#include "pingpong.h"
#include "texture.h"
#include "framebuffer.h"
#include "context.h"
#include "uniform.h"
#include "debug.h"

using namespace monad;

PingPong::PingPong(Context *context, Texture *texA, Texture *texB) : texA(texA), texB(texB), initialTexA(texA), initialTexB(texB) {
    TextureImage imgs[] = { texA->getImage(0) };
    fbA = initialFbA = context->createFramebuffer(1, imgs);
    imgs[0] = texB->getImage(0);
    fbB = initialFbB = context->createFramebuffer(1, imgs);
}

PingPong::~PingPong() {
    delete fbA;
    delete fbB;
}

void PingPong::swap() {
    auto oldTexA = texA;
    texA = texB;
    texB = oldTexA;

    auto oldFbA = fbA;
    fbA = fbB;
    fbB = oldFbA;
}

void PingPong::reset() {
    fbA = initialFbA;
    fbB = initialFbB;
    texA = initialTexA;
    texB = initialTexB;
}

void PingPong::bind(const ivec4 &viewport, Uniform<int> *sampler) {
    sampler->setVal(getReadTex()->unit);
    getWriteFramebuffer()->bind(viewport);
}

void PingPong::swapAndBind(const ivec4 &viewport, Uniform<int> *sampler) {
    dbgEnter("swapAndBind");
    swap();
    bind(viewport, sampler);
    dbgExit();
}

void PingPong::resetAndBind(const ivec4 &viewport, Uniform<int> *sampler) {
    dbgEnter("resetAndBind");
    reset();
    bind(viewport, sampler);
    dbgExit();
}
