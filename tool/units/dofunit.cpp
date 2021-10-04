#include "dofunit.h"
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

DofUnit::DofUnit(Context *context) : context(context) {
    dbgEnter("DofUnit constructor");
    timer = context->createTimer("DoF");

    auto frag = context->getFragmentShader("post/dof.frag");
    program = context->createPostProgram(frag);
    program->complete();
    colorSampler = program->getUniform<int>("sColor", 0);
    depthSampler = program->getUniform<int>("sDepth", 0);
    focusRange = program->getUniform<vec2>("focusRange", {0, 0});
    focusGradient = program->getUniform<float>("focusGradient", 0);
    focusSize = program->getUniform<float>("focusSize", 0);

    program->getUniform<vec2>("viewportSize", vec2(context->getViewSize()));

    outColor = new Texture("DoF color", GL_RGB32F, GL_RGB);
    auto img = outColor->addImage(context->getViewSize());
    fb = context->createFramebuffer(1, &img);

    focusRangeTrack = context->getTrack<vec2>("post:dofRange");
    focusGradientTrack = context->getTrack<float>("post:dofGradient");
    focusSizeTrack = context->getTrack<float>("post:dofSize");
    dbgExit();
}

void DofUnit::setInputs(Texture *color, Texture *depth) {
    inColor = color;
    inDepth = depth;

    colorSampler->setVal(color->unit);
    depthSampler->setVal(depth->unit);
}

void DofUnit::render() {
    dbgEnter("DofUnit render");
    timer->start();

    focusRange->setVal(focusRangeTrack->getValue());
    focusGradient->setVal(focusGradientTrack->getValue());
    focusSize->setVal(focusSizeTrack->getValue());

    fb->bind(context->getViewport());
    context->getQuad()->render(program);

    timer->stop();
    dbgExit();
}
