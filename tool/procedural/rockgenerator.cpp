#include "rockgenerator.h"
#include "renderer/context.h"
#include "renderer/rawobject.h"
#include "renderer/feedbackobject.h"
#include "renderer/program.h"
#include "debug.h"

using namespace monad;

static const char *varyingNames[] = { "position", "normal" };
static const float rockPatchData[] = { 0, 0, 0 };

RockGenerator::RockGenerator(Context *context) : context(context) {
    _rockPatch = context->createRawObject();
    _rockPatch->setData(1, sizeof(rockPatchData), rockPatchData);
    _rockPatch->attrib(3, 3, 0);

    _rockProgram = context->createProgram();
    _rockProgram->attachShader(context->getVertexShader("rock/generator.vert"));
    _rockProgram->attachShader(context->getTessControlShader("rock/generator.tesc"));
    _rockProgram->attachShader(context->getTessEvaluationShader("rock/generator.tese"));
    _rockProgram->attachShader(context->getGeometryShader("rock/generator.geom"));
    glTransformFeedbackVaryings(_rockProgram->resource, 1, varyingNames, GL_INTERLEAVED_ATTRIBS);
    _rockProgram->complete();
    _seedUniform = _rockProgram->getUniform<float>("seed", 0);
}

FeedbackObject *RockGenerator::generate() {
    dbgEnter("Generate rock vertices");
    auto r = rand();
    //printf("generate=%i\n", r);
    _seedUniform->setVal(r / (float)RAND_MAX);
    auto obj = context->createFeedbackObject(4096*3*8, _rockPatch);
    glPatchParameteri(GL_PATCH_VERTICES, 1);
    obj->renderFeedback(_rockProgram);
    obj->attrib(3, 6, 0);
    obj->attrib(3, 6, 3);
    dbgExit();
    return obj;
}
