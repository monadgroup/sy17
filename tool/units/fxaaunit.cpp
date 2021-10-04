#include "fxaaunit.h"
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

FxaaUnit::FxaaUnit(Context *context) : context(context) {
    dbgEnter("FxaaUnit constructor");
    timer = context->createTimer("FXAA");

    auto frag = context->getFragmentShader("post/fxaa.frag");
    program = context->createPostProgram(frag);
    program->complete();
    colorSampler = program->getUniform<int>("sColor", 0);

    program->getUniform<vec2>("viewportSize", vec2(context->getViewSize()));
    dbgExit();
}

void FxaaUnit::setInputs(Texture *color) {
    inColor = color;

    colorSampler->setVal(color->unit);
}

void FxaaUnit::render() {
    dbgEnter("FxaaUnit render");
    timer->start();

    context->getScreenFramebuffer()->bind(context->getScreenport());
    context->getQuad()->render(program);

    timer->stop();
    dbgExit();
}
