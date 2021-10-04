#include "directionallightunit.h"
#include "renderer/context.h"
#include "renderer/texture.h"
#include "renderer/program.h"
#include "renderer/framebuffer.h"
#include "renderer/uniform.h"
#include "renderer/rawobject.h"
#include "renderer/pingpong.h"
#include "renderer/directionallight.h"
#include "renderer/timer.h"
#include "debug.h"

using namespace monad;

DirectionalLightUnit::DirectionalLightUnit(Context *context, ivec2 resolution) : context(context), resolution(resolution) {
    dbgEnter("DirectionalLightUnit constructor");
    timer = context->createTimer("Lighting");

    auto frag = context->getFragmentShader("gauss21.frag");
    program = context->createPostProgram(frag);
    program->complete();
    colorSampler = program->getUniform<int>("inTex", 0);
    blurDirection = program->getUniform<vec2>("direction", {0, 0});

    program->getUniform<vec2>("viewportSize", vec2(resolution));

    auto depthTex = new Texture("Directional depth", GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
    auto depthImg = depthTex->addImage(resolution);
    auto pingTex = new Texture("Directional ping", GL_RG32F, GL_RG);
    pingTex->addImage(resolution);
    auto pongTex = new Texture("Directional pong", GL_RG32F, GL_RG);
    pongTex->addImage(resolution);

    pingPong = new PingPong(context, pingTex, pongTex);
    TextureImage fbImages[] = {
            TextureImage(),
            TextureImage(),
            TextureImage(),
            TextureImage(),
            pingPong->getReadTex()->getImage(0)
    };
    renderFb = context->createFramebuffer(5, fbImages, &depthImg);
    renderFb->clearColor = {1, 1, 1, 1};
    dbgExit();
}

void DirectionalLightUnit::setLight(SHARED_PTR(DirectionalLight) light) {
    this->light = light;
}

void DirectionalLightUnit::render(IRenderable *renderable) {
    dbgEnter("DirectionalLightUnit render");
    timer->start();

    light->update();
    renderable->render(RenderType::LIGHTING, renderFb, {0, 0, resolution.x, resolution.y});

    pingPong->resetAndBind({0, 0, resolution.x, resolution.y}, colorSampler);
    blurDirection->setVal({1, 0});
    context->getQuad()->render(program);
    pingPong->swapAndBind({0, 0, resolution.x, resolution.y}, colorSampler);
    blurDirection->setVal({0, 1});
    context->getQuad()->render(program);

    timer->stop();
    dbgExit();
}

Texture* DirectionalLightUnit::getVarianceTexture() {
    return pingPong->getReadTex();
}
