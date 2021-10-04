#include "context.h"
#include "debug.h"
#include "renderer/program.h"
#include "renderer/shader.h"
#include "time/timemanager.h"
#include "rawobject.h"
#include "scene.h"
#include "object.h"
#include "feedbackobject.h"
#include "metadata.h"
#include "procedural/noisegenerator.h"
#include "procedural/diamondsquaregenerator.h"
#include "procedural/rockgenerator.h"
#include "units/sceneunit.h"
#include "units/postunit.h"

using namespace monad;

Context::Context(TimeManager *manager, demo::DemoWindow *window, ivec2 size) : manager(manager) {
    dbgEnter("Context constructor");

    // set window size, contrain to aspect ratio
    screenSize = size;
    vec2 newView = { screenSize.y * meta::aspect, (float)screenSize.y };
    if (newView.x > screenSize.x) newView = { (float)screenSize.x, screenSize.x / meta::aspect };

    viewSize = {(int)newView.x, (int)newView.y};
    viewOffset = {(int)(screenSize.x / 2.0f - newView.x / 2.0f), (int)(screenSize.y / 2.0f - newView.y / 2.0f)};

    // build post vao
    dbgEnter("Build post-processing VAO");
    static const float quadBufferData[] = {
        -1.f, -1.f,
        1.f, -1.f,
        -1.f, 1.f,
        -1.f, 1.f,
        1.f, -1.f,
        1.f, 1.f
    };
    postObject = createRawObject(GL_STATIC_DRAW, GL_TRIANGLES);
    postObject->setData(6, sizeof(quadBufferData), quadBufferData);
    postObject->attrib(2, 2, 0);
    dbgExit();

    // build object vao
    dbgEnter("Build object VAO");
    glGenVertexArrays(1, &objectVao);
    glBindVertexArray(objectVao);

    // position
    glVertexAttribFormat(0, 3, GL_FLOAT, FALSE, 0);
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribFormat(1, 3, GL_FLOAT, FALSE, 3 * sizeof(float));
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(1);

    // material
    glVertexAttribFormat(2, 1, GL_FLOAT, FALSE, 6 * sizeof(float));
    glVertexAttribBinding(2, 0);
    glEnableVertexAttribArray(2);
    dbgExit();

    // get common shaders
    rasterFragmentShader = getFragmentShader("raster.frag");
    rasterVertexShader = getVertexShader("raster.vert");
    postVertexShader = getVertexShader("identity2.vert");

    // get object program
    objectProgram = createProgram(rasterVertexShader, rasterFragmentShader);
    objectProgram->complete();

    // create generators
    noiseGenerator = new NoiseGenerator();
    diamondSquareGenerator = new DiamondSquareGenerator();

    marsTerrain = diamondSquareGenerator->generate({4096, 4096}, 8192);

    sceneUnit = new SceneUnit(this);
    postUnit = new PostUnit(this);
    postUnit->setInputs(sceneUnit->getColorTexture(), sceneUnit->getDepthTexture(), sceneUnit->getVelocityTexture(), sceneUnit->getHighlightsTexture());

    dbgExit();
}

void Context::renderFrame(Scene &scene, const ivec2 &screenSize) {
    scene.frame();
}

SHARED_PTR(VertexShader) Context::getVertexShader(const char *path) const {
    return getShader(path, GL_VERTEX_SHADER);
}

SHARED_PTR(TessControlShader) Context::getTessControlShader(const char *path) const {
    return getShader(path, GL_TESS_CONTROL_SHADER);
}

SHARED_PTR(TessEvaluationShader) Context::getTessEvaluationShader(const char *path) const {
    return getShader(path, GL_TESS_EVALUATION_SHADER);
}

SHARED_PTR(GeometryShader) Context::getGeometryShader(const char *path) const {
    return getShader(path, GL_GEOMETRY_SHADER);
}

SHARED_PTR(FragmentShader) Context::getFragmentShader(const char *path) const {
    return getShader(path, GL_FRAGMENT_SHADER);
}

SHARED_PTR(ComputeShader) Context::getComputeShader(const char *path) const {
    return getShader(path, GL_COMPUTE_SHADER);
}

SHARED_PTR(Program) Context::createProgram() const {
    return MAKE_SHARED(Program, glCreateProgram());
}

SHARED_PTR(Program) Context::createProgram(SHARED_PTR(VertexShader) vertexShader, SHARED_PTR(FragmentShader) fragmentShader) const {
    auto program = createProgram();
    program->attachShader(std::move(vertexShader));
    program->attachShader(std::move(fragmentShader));
    return program;
}

SHARED_PTR(Program) Context::createPostProgram(SHARED_PTR(FragmentShader) fragmentShader) const {
    auto program = createProgram();
    program->attachShader(postVertexShader);
    program->attachShader(std::move(fragmentShader));
    return program;
}

SHARED_PTR(DirectionalLight) Context::createDirectionalLight(Track<vec3> *tLightPos,
                                                             Track<ForwardDirection> *tLightDir,
                                                             Track<vec2> *tHorizontalRange, Track<vec2> *tVerticalRange,
                                                             Track<vec2> *tDepthRange) {
    return MAKE_SHARED(DirectionalLight, tLightPos, tLightDir, tHorizontalRange, tVerticalRange, tDepthRange);
}

RawObject* Context::createRawObject(GLenum usageMode, GLenum renderMode) const {
    GLuint buffer, vao;
    glGenBuffers(1, &buffer);
    glGenVertexArrays(1, &vao);
    return new RawObject(buffer, vao, usageMode, renderMode);
}

Object *Context::createObject(Mesh *mesh, GLenum usageMode, GLenum renderMode) const {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return new Object(buffer, objectVao, mesh, usageMode, renderMode);
}

FeedbackObject* Context::createFeedbackObject(int bufferSize, RawObject *object) const {
    GLuint buffer, vao, feedback;
    glGenBuffers(1, &buffer);
    glGenVertexArrays(1, &vao);
    glGenTransformFeedbacks(1, &feedback);
    return new FeedbackObject(buffer, vao, feedback, bufferSize, object);
}

Framebuffer* Context::createFramebuffer(int colorCount, const TextureImage *colors, const TextureImage *depth, const TextureImage *stencil) const {
    GLuint buffer;
    glGenFramebuffers(1, &buffer);
    return new Framebuffer(buffer, colorCount, colors, depth, stencil);
}

void Context::runPipeline(const SHARED_PTR(Program) shadeProgram, IRenderable *renderable) {
    sceneUnit->render(shadeProgram, renderable);
    postUnit->render();
}

float Context::getTime() {
    return (float)manager->getRow();
}
