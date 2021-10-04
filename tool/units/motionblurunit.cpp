#include "motionblurunit.h"
#include "renderer/context.h"
#include "renderer/texture.h"
#include "renderer/program.h"
#include "renderer/framebuffer.h"
#include "renderer/uniform.h"
#include "renderer/rawobject.h"
#include "renderer/timer.h"
#include "debug.h"

using namespace monad;

MotionBlurUnit::MotionBlurUnit(Context *context) : context(context) {
    dbgEnter("MotionBlurUnit constructor");
    timer = context->createTimer("Motion blur");

    auto frag = context->getFragmentShader("post/motionblur.frag");
    program = context->createPostProgram(frag);
    program->complete();
    colorSampler = program->getUniform<int>("sColor", 0);
    depthSampler = program->getUniform<int>("sDepth", 0);
    velocitySampler = program->getUniform<int>("sVelocity", 0);

    outColor = new Texture("Motion blur color", GL_RGB32F, GL_RGB);
    auto img = outColor->addImage(context->getViewSize());
    fb = context->createFramebuffer(1, &img);
    dbgExit();
}

void MotionBlurUnit::setInputs(Texture *color, Texture *depth, Texture *velocity) {
    inColor = color;
    inDepth = depth;
    inVelocity = velocity;

    colorSampler->setVal(color->unit);
    depthSampler->setVal(depth->unit);
    velocitySampler->setVal(velocity->unit);
}

void MotionBlurUnit::render() {
    dbgEnter("MotionBlurUnit render");
    timer->start();

    fb->bind(context->getViewport());
    context->getQuad()->render(program);

    timer->stop();
    dbgExit();
}
