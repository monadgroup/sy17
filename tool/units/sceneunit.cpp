#include "sceneunit.h"
#include "renderer/context.h"
#include "renderer/texture.h"
#include "renderer/program.h"
#include "renderer/framebuffer.h"
#include "renderer/uniform.h"
#include "renderer/rawobject.h"
#include "renderer/camera.h"
#include "renderer/defines.h"
#include "renderer/timer.h"
#include "time/track_.h"
#include "debug.h"

using namespace monad;

ivec2 SceneUnit::lightResolution = {1024, 1024};

SceneUnit::SceneUnit(Context *context)
        : context(context), directionalLight(context, lightResolution), ssao(context), ubo(GL_DYNAMIC_DRAW) {
    dbgEnter("SceneUnit constructor");
    sceneTimer = context->createTimer("Scene");
    shadeTimer = context->createTimer("Shade");

    auto screenSize = context->getViewSize();

    posMaterialTex = new Texture("Scene pos/material", GL_RGBA32F, GL_RGBA);
    auto posMaterialImage = posMaterialTex->addImage(screenSize);
    normalTex = new Texture("Scene normal", GL_RGB32F, GL_RGB);
    auto normalImage = normalTex->addImage(screenSize);
    localPosTex = new Texture("Scene local pos", GL_RGB32F, GL_RGB);
    auto localPosImage = localPosTex->addImage(screenSize);
    velocityTex = new Texture("Scene velocity", GL_RGB32F, GL_RGB);
    auto velocityImage = velocityTex->addImage(screenSize);
    depthTex = new Texture("Scene depth", GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
    auto depthImage = depthTex->addImage(screenSize);
    TextureImage renderColors[] = {
        posMaterialImage,
        normalImage,
        localPosImage,
        velocityImage
    };
    renderFb = context->createFramebuffer(4, renderColors, &depthImage);

    colorTex = new Texture("Shade color", GL_RGB32F, GL_RGB);
    auto colorImage = colorTex->addImage(screenSize);
    highlightsTex = new Texture("Shade highlights", GL_RGB32F, GL_RGB);
    auto highlightsImage = highlightsTex->addImage(screenSize);
    TextureImage shadeColors[] = {
        colorImage,
        highlightsImage
    };
    shadeFb = context->createFramebuffer(2, shadeColors);

    ssao.setInputs(posMaterialTex, depthTex, normalTex);
    ubo.setBinding(SCENE_BUFFER_BINDING);
    dbgExit();
}

void SceneUnit::prepareProgram(SHARED_PTR(Program) program) {
    program->setSampler("sPosMaterial", posMaterialTex);
    program->setSampler("sNormal", normalTex);
    program->setSampler("sLocalPos", localPosTex);
    program->setSampler("sDepth", depthTex);
    program->setSampler("sLightDepth", directionalLight.getVarianceTexture());
    program->setSampler("sAo", ssao.getColorTexture());
}

void SceneUnit::setCamera(SHARED_PTR(Camera) camera) {
    this->camera = camera;
}

void SceneUnit::setLight(SHARED_PTR(DirectionalLight) light) {
    directionalLight.setLight(light);
}

void SceneUnit::render(const SHARED_PTR(Program) program, IRenderable *renderable) {
    dbgEnter("SceneUnit render");
    // update scene data
    auto data = context->getTime();
    glBindBuffer(GL_UNIFORM_BUFFER, ubo.resource);
    glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(float), &data, ubo.mode);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    directionalLight.render(renderable);

    sceneTimer->start();
    camera->update(context->getViewAspect());
    renderable->render(RenderType::COLOR, renderFb, context->getViewport());
    sceneTimer->stop();

    ssao.render();

    shadeTimer->start();
    shadeFb->bind(context->getViewport());
    context->getQuad()->render(program);
    shadeTimer->stop();
    dbgExit();
}
