#include "util.h"
#include "ssaounit.h"
#include "renderer/context.h"
#include "renderer/texture.h"
#include "renderer/program.h"
#include "renderer/framebuffer.h"
#include "renderer/uniform.h"
#include "renderer/rawobject.h"
#include "renderer/timer.h"
#include "time/track_.h"
#include "debug.h"

using namespace monad;

SsaoUnit::SsaoUnit(Context *context) : context(context) {
    dbgEnter("SsaoUnit constructor");
    timer = context->createTimer("SSAO");

    // generate noise data
    auto noiseSize = 4;
    auto noiseDataSize = noiseSize * noiseSize;
    List<vec3> noise = List<vec3>(noiseDataSize);
    for (auto i = 0; i < noiseDataSize; i++) {
        vec3 sample = {
            random(-1, 1),
            random(-1, 1),
            0
        };
        noise.push_back(sample);
    }
    auto noiseTex = new Texture("SSAO Noise", GL_RGB16F, GL_RGB, GL_FLOAT, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
    noiseTex->setImage(0, {noiseSize, noiseSize}, noise.begin());

    // generate sample data
    auto kernelSize = 64;
    List<vec3> samples = List<vec3>(kernelSize);
    for (auto i = 0; i < kernelSize; i++) {
        vec3 sample = {
            random(-1, 1),
            random(-1, 1),
            random(0, 1)
        };
        sample = sample.unit();
        float scale = (float)i / kernelSize;
        scale = lerpf(0.1f, 1.0f, scale*scale);
        samples.push_back(sample * scale);
    }

    // generator program
    auto genFrag = context->getFragmentShader("ssao.frag");
    genProgram = context->createPostProgram(genFrag);
    genProgram->complete();
    posSampler = genProgram->getUniform<int>("sPosMaterial", 0);
    depthSampler = genProgram->getUniform<int>("sDepth", 0);
    normalSampler = genProgram->getUniform<int>("sNormal", 0);

    genProgram->setSampler("sAoNoise", noiseTex);
    genProgram->getUniform<List<vec3>>("aoKernel", samples);
    genProgram->getUniform<vec2>("aoScale", vec2(context->getViewSize()) / 4);

    radius = genProgram->getUniform<float>("aoRadius", 0);
    power = genProgram->getUniform<float>("aoPower", 0);

    auto generatedColor = new Texture("SSAO gen color", GL_R16F, GL_RED);
    auto generatedImage = generatedColor->addImage(context->getViewSize());
    genFb = context->createFramebuffer(1, &generatedImage);

    // blur program
    auto blurFrag = context->getFragmentShader("box4.frag");
    blurProgram = context->createPostProgram(blurFrag);
    blurProgram->complete();
    blurProgram->setSampler("inTex", generatedColor);

    blurProgram->getUniform<vec2>("viewportSize", vec2(context->getViewSize()));

    outColor = new Texture("SSAO blur color", GL_R16F, GL_RED);
    auto outImage = outColor->addImage(context->getViewSize());
    blurFb = context->createFramebuffer(1, &outImage);

    radiusTrack = context->getTrack<float>("post:aoRadius");
    powerTrack = context->getTrack<float>("post:aoPower");
    dbgExit();
}

void SsaoUnit::setInputs(Texture *pos, Texture *depth, Texture *normal) {
    inPos = pos;
    inDepth = depth;
    inNormal = normal;

    posSampler->setVal(pos->unit);
    depthSampler->setVal(depth->unit);
    normalSampler->setVal(normal->unit);
}

void SsaoUnit::render() {
    dbgEnter("SsaoUnit render");
    timer->start();

    radius->setVal(radiusTrack->getValue());
    power->setVal(powerTrack->getValue());

    genFb->bind(context->getViewport());
    context->getQuad()->render(genProgram);

    blurFb->bind(context->getViewport());
    context->getQuad()->render(blurProgram);

    timer->stop();
    dbgExit();
}
