#include "rover.h"
#include "includes.h"

using namespace monad;

void RoverScene::init() {
    auto testVertex = context->getVertexShader("__test/test.vert");
    auto testFrag = context->getFragmentShader("__test/test.frag");

    auto terrainVert = context->getVertexShader("__test/terraintest/terrain.vert");
    auto terrainTec = context->getTessControlShader("__test/terraintest/terrain.tesc");
    auto terrainTee = context->getTessEvaluationShader("__test/terraintest/terrain.tese");
    auto terrainFrag = context->getFragmentShader("__test/terraintest/terrain.frag");

    auto postFrag = context->getFragmentShader("__test/post.frag");

    // create program
    _program = context->createProgram(testVertex, testFrag);
    _program->complete();

    _tessProgram = context->createProgram(terrainVert, terrainFrag);
    _tessProgram->attachShader(terrainTec);
    _tessProgram->attachShader(terrainTee);
    _tessProgram->complete();

    _postProgram = context->createPostProgram(postFrag);
    _postProgram->complete();

    // get uniforms
    _tessLevelInner = _tessProgram->getUniform<float>("TessLevelInner", 5);
    _tessLevelOuter = _tessProgram->getUniform<float>("TessLevelOuter", 5);

    // set samplers
    _postProgram->setSampler("inTex", context->getSceneColorTex());
    _postProgram->setSampler("inDepth", context->getSceneDepthTex());

    // get tracks
    _camera = context->createCamera(
        context->getTrack<vec3>("camera.pos"),
        context->getTrack<ForwardDirection>("camera.gymbal"),
        context->getTrack<float>("camera.length"),
        context->getTrack<Direction>("camera.dir"),
        context->getTrack<float>("camera.fov"),
        context->getTrack<vec2>("camera.clipRange")
    );

    // create meshes
    _boxObject = context->createObject(gen::box(0, {1, 1, 1}), _program);
    _quadObject = context->createObject(gen::quad(1, {10, 10}), _tessProgram);
    _quadObject->renderMode = GL_PATCHES;
}

void RoverScene::enter() {

}

void RoverScene::frame() {
    _camera->update(context->getViewAspect());

    glEnable(GL_DEPTH_TEST);
    context->setSceneFbo();

    _boxObject->startRender();
    for (auto i = -4; i <= 4; i++) {
        _boxObject->setTransform(Matrix4::move({i * 3.f, 0, 0}));
        _boxObject->doRender();
    }
    _boxObject->endRender();

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    _quadObject->render();

    // render post
    glDisable(GL_DEPTH_TEST);
    context->setScreenFbo();
    context->renderQuad(_postProgram);
}

void RoverScene::leave() {

}

