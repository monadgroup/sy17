#include "compositeunit.h"
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

CompositeUnit::CompositeUnit(Context *context) : context(context) {
    dbgEnter("CompositeUnit constructor");
    timer = context->createTimer("Composite");

    auto frag = context->getFragmentShader("post/composite.frag");
    program = context->createPostProgram(frag);
    program->complete();
    colorSampler = program->getUniform<int>("sColor", 0);
    bloomSampler = program->getUniform<int>("sBloom", 0);
    flareSampler = program->getUniform<int>("sFlare", 0);
    exposure = program->getUniform<float>("exposure", 0);
    vignetteStrength = program->getUniform<float>("vignetteStrength", 0);
    vignetteSize = program->getUniform<float>("vignetteSize", 0);
    vignettePower = program->getUniform<float>("vignettePower", 0);
    grainAmount = program->getUniform<float>("grainAmount", 0);

    outColor = new Texture("Composite color");
    auto img = outColor->addImage(context->getViewSize());
    fb = context->createFramebuffer(1, &img);

    exposureTrack = context->getTrack<float>("post:exposure");
    vignetteStrengthTrack = context->getTrack<float>("post:vignetteStrength");
    vignetteSizeTrack = context->getTrack<float>("post:vignetteSize");
    vignettePowerTrack = context->getTrack<float>("post:vignettePower");
    grainAmountTrack = context->getTrack<float>("post:grain");
    dbgExit();
}

void CompositeUnit::setInputs(Texture *color, Texture *bloom, Texture *flare) {
    inColor = color;
    inBloom = bloom;
    inFlare = flare;

    colorSampler->setVal(color->unit);
    bloomSampler->setVal(bloom->unit);
    flareSampler->setVal(flare->unit);
}

void CompositeUnit::render() {
    dbgEnter("CompositeUnit render");
    timer->start();

    exposure->setVal(exposureTrack->getValue());
    vignetteStrength->setVal(vignetteStrengthTrack->getValue());
    vignetteSize->setVal(vignetteSizeTrack->getValue());
    vignettePower->setVal(vignettePowerTrack->getValue());
    grainAmount->setVal(grainAmountTrack->getValue());

    fb->bind(context->getViewport());
    context->getQuad()->render(program);

    timer->stop();
    dbgExit();
}
