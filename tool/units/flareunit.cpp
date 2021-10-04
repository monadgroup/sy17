#include "flareunit.h"
#include "renderer/context.h"
#include "renderer/framebuffer.h"
#include "renderer/uniform.h"
#include "renderer/program.h"
#include "time/track_.h"
#include "renderer/pingpong.h"
#include "renderer/rawobject.h"
#include "renderer/pingpong.h"
#include "renderer/timer.h"
#include "debug.h"

using namespace monad;

FlareUnit::FlareUnit(Context *context) : context(context) {
    dbgEnter("FlareUnit constructor");
    timer = context->createTimer("Flare");
    size = context->getViewSize() / 2;

    downsampled = new Texture("Flare downsample", GL_RGB32F, GL_RGB);
    auto downsampledImage = downsampled->addImage(size);
    downsampleFb = context->createFramebuffer(1, &downsampledImage);

    auto downsampleFrag = context->getFragmentShader("post/flaredownsample.frag");
    downsampleProgram = context->createPostProgram(downsampleFrag);
    downsampleProgram->complete();
    colorSampler = downsampleProgram->getUniform<int>("sColor", 0);
    scale = downsampleProgram->getUniform<float>("scale", 0);
    bias = downsampleProgram->getUniform<float>("bias", 0);

    auto effectFrag = context->getFragmentShader("post/flaregen.frag");
    effectProgram = context->createPostProgram(effectFrag);
    effectProgram->complete();
    effectProgram->setSampler("sColor", downsampled);
    ghostCount = effectProgram->getUniform<int>("ghostCount", 0);
    ghostDispersal = effectProgram->getUniform<float>("ghostDispersal", 0);
    haloWidth = effectProgram->getUniform<float>("haloWidth", 0);

    effectProgram->getUniform<vec2>("viewportSize", vec2(size));

    auto pingTex = new Texture("Flare ping", GL_RGB32F, GL_RGB);
    pingTex->addImage(size);
    auto pongTex = new Texture("Flare pong", GL_RGB32F, GL_RGB);
    pongTex->addImage(size);
    pingPong = new PingPong(context, pingTex, pongTex);

    auto blurFrag = context->getFragmentShader("gauss21.frag");
    blurProgram = context->createPostProgram(blurFrag);
    blurProgram->complete();
    blurSampler = blurProgram->getUniform<int>("inTex", 0);
    blurDirection = blurProgram->getUniform<vec2>("direction", {0, 0});

    blurProgram->getUniform<vec2>("viewportSize", vec2(size));

    scaleTrack = context->getTrack<float>("post:flareScale");
    biasTrack = context->getTrack<float>("post:flareBias");
    ghostCountTrack = context->getTrack<int>("post:flareCount");
    ghostDispersalTrack = context->getTrack<float>("post:flareDispersal");
    haloWidthTrack = context->getTrack<float>("post:flareHalo");
    dbgExit();
}

void FlareUnit::setInputs(Texture *color) {
    inColor = color;
    colorSampler->setVal(color->unit);
}

void FlareUnit::render() {
    dbgEnter("FlareUnit render");
    timer->start();

    scale->setVal(scaleTrack->getValue());
    bias->setVal(biasTrack->getValue());
    ghostCount->setVal(ghostCountTrack->getValue());
    ghostDispersal->setVal(ghostDispersalTrack->getValue());
    haloWidth->setVal(haloWidthTrack->getValue());

    downsampleFb->bind({0, 0, size.x, size.y});
    context->getQuad()->render(downsampleProgram);

    pingPong->reset();
    pingPong->getReadFramebuffer()->bind({0, 0, size.x, size.y});
    context->getQuad()->render(effectProgram);

    /*pingPong->bind({0, 0, size.x, size.y}, blurSampler);
    blurDirection->setVal({1, 0});
    context->getQuad()->render(blurProgram);
    pingPong->swapAndBind({0, 0, size.x, size.y}, blurSampler);
    blurDirection->setVal({0, 1});
    context->getQuad()->render(blurProgram);*/
    timer->stop();
    dbgExit();
}

Texture* FlareUnit::getColorTexture() const {
    return pingPong->getReadTex();
}
