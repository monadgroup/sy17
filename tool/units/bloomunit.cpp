#include "bloomunit.h"
#include "renderer/context.h"
#include "renderer/framebuffer.h"
#include "renderer/uniform.h"
#include "renderer/program.h"
#include "time/track_.h"
#include "renderer/pingpong.h"
#include "renderer/rawobject.h"
#include "renderer/timer.h"
#include "debug.h"

using namespace monad;

BloomUnit::BloomUnit(Context *context, int iterationCount) : context(context)
{
    dbgEnter("BloomUnit constructor");
    timer = context->createTimer("Bloom");

    List<int> colorUnits = List<int>(unitCount);
    auto currentSize = context->getViewSize();
    for (auto &unit : units) {
        unit = new BloomBlurUnit(context, currentSize, iterationCount, GL_RGB32F, GL_RGB);
        currentSize = currentSize / 2;

        colorUnits.push_back(unit->getColorTexture()->unit);
    }

    auto copyFrag = context->getFragmentShader("copy.frag");
    copyProgram = context->createPostProgram(copyFrag);
    copyProgram->complete();
    copySampler = copyProgram->getUniform<int>("sIn", 0);

    auto blurFrag = context->getFragmentShader("post/bloomblur.frag");
    program = context->createPostProgram(blurFrag);
    program->complete();
    colorSampler = program->getUniform<int>("sColor", 0);
    direction = program->getUniform<vec2>("glowDirection", {0, 0});
    viewportSize = program->getUniform<vec2>("viewportSize", {0, 0});

    auto compositeFrag = context->getFragmentShader("post/bloomcomposite.frag");
    compositeProgram = context->createPostProgram(compositeFrag);
    compositeProgram->complete();
    compositeProgram->getUniform<List<int>>("sBlooms", colorUnits);
    bloomBias = compositeProgram->getUniform<float>("bloomBias", 0);
    bloomPower = compositeProgram->getUniform<float>("bloomPower", 0);
    bloomAmount = compositeProgram->getUniform<float>("bloomAmount", 0);

    outColor = new Texture("Bloom composite", GL_RGB32F, GL_RGB);
    auto colorImage = outColor->addImage(context->getViewSize());
    compositeFb = context->createFramebuffer(1, &colorImage);

    bloomBiasTrack = context->getTrack<float>("post:bloomBias");
    bloomPowerTrack = context->getTrack<float>("post:bloomPower");
    bloomAmountTrack = context->getTrack<float>("post:bloomAmount");

    dbgExit();
}

void BloomUnit::setInputs(Texture *highlights) {
    inHighlights = highlights;
}

void BloomUnit::render() {
    dbgEnter("BloomUnit render");
    timer->start();

    bloomBias->setVal(bloomBiasTrack->getValue());
    bloomPower->setVal(bloomPowerTrack->getValue());
    bloomAmount->setVal(bloomAmountTrack->getValue());

    // downsample through the tree
    auto lastTex = inHighlights->unit;
    for (auto &unit : units) {
        copySampler->setVal(lastTex);
        unit->bindSourceFramebuffer();
        context->getQuad()->render(copyProgram);
    }

    // pingpong each unit
    for (auto &unit : units) {
        viewportSize->setVal(vec2(unit->getViewport()));
        unit->render(program, colorSampler, direction);
    }

    // composite
    compositeFb->bind(context->getViewport());
    context->getQuad()->render(compositeProgram);

    timer->stop();
    dbgExit();
}
