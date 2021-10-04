#include "postunit.h"
#include "renderer/texture.h"
#include "renderer/context.h"
#include "debug.h"

using namespace monad;

PostUnit::PostUnit(Context *context)
        : context(context),
          bloom(context),
          dof(context),
          motionBlur(context),
          flare(context),
          composite(context),
          fxaa(context) {
    dbgEnter("PostUnit constructor");
    flare.setInputs(bloom.getColorTexture());
    composite.setInputs(motionBlur.getColorTexture(), bloom.getColorTexture(), flare.getColorTexture());
    fxaa.setInputs(composite.getColorTexture());
    dbgExit();
}

void PostUnit::setInputs(Texture *color, Texture *depth, Texture *velocity, Texture *highlights) {
    bloom.setInputs(highlights);
    dof.setInputs(color, depth);
    motionBlur.setInputs(dof.getColorTexture(), depth, velocity);
}

void PostUnit::render() {
    dbgEnter("PostUnit render");
    bloom.render();
    dof.render();
    motionBlur.render();
    flare.render();
    composite.render();
    fxaa.render();
    dbgExit();
}
