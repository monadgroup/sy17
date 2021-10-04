#include "bloomblurunit.h"
#include "flareunit.h"
#include "renderer/context.h"
#include "renderer/framebuffer.h"
#include "renderer/uniform.h"
#include "renderer/program.h"
#include "time/track_.h"
#include "renderer/pingpong.h"
#include "renderer/rawobject.h"
#include "renderer/pingpong.h"
#include "debug.h"

using namespace monad;

BloomBlurUnit::BloomBlurUnit(Context *context, ivec2 viewport, int iterationCount, GLenum internalFormat, GLenum format)
        : context(context), viewport(viewport), iterationCount(iterationCount) {
    dbgEnter("BloomBlurUnit constructor");
    auto pingTex = new Texture("Bloom blur ping", internalFormat, format);
    pingTex->addImage(viewport);
    auto pongTex = new Texture("Bloom blur pong", internalFormat, format);
    pongTex->addImage(viewport);
    pingPong = new PingPong(context, pingTex, pongTex);
    dbgExit();
}

void BloomBlurUnit::bindSourceFramebuffer() {
    pingPong->getBaseReadFramebuffer()->bind({0, 0, viewport.x, viewport.y});
}

void BloomBlurUnit::render(const SHARED_PTR(Program) program, Uniform<int> *sampler, Uniform<vec2> *direction) {
    dbgEnter("BloomBlurUnit render");
    for (auto i = 0; i < iterationCount; i++) {
        pingPong->resetAndBind({0, 0, viewport.x, viewport.y}, sampler);
        direction->setVal({1, 0});
        context->getQuad()->render(program);
        pingPong->swapAndBind({0, 0, viewport.x, viewport.y}, sampler);
        direction->setVal({0, 1});
        context->getQuad()->render(program);
    }
    dbgExit();
}

Texture* BloomBlurUnit::getColorTexture() const {
    return pingPong->getBaseReadTex();
}
