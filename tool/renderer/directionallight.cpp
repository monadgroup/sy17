#include "directionallight.h"
#include "time/track_.h"
#include "defines.h"
#include "debug.h"

using namespace monad;

DirectionalLight::DirectionalLight(Track<vec3> *tLightPos, Track<ForwardDirection> *tLightDir,
                                   Track<vec2> *tHorizontalRange, Track<vec2> *tVerticalRange,
                                   Track<vec2> *tDepthRange)
    : _tLightPos(tLightPos), _tLightDir(tLightDir), _tHorizontalRange(tHorizontalRange), _tVerticalRange(tVerticalRange),
      _tDepthRange(tDepthRange), cameraUbo(GL_DYNAMIC_DRAW), lightUbo(GL_DYNAMIC_DRAW) {

}

vec3 DirectionalLight::getPosition() const {
    return _tLightPos->getValue();
}

ForwardDirection DirectionalLight::getDirection() const {
    return _tLightDir->getValue();
}

Matrix4 DirectionalLight::getViewMatrix() const {
    auto position = getPosition();
    auto direction = getDirection();
    return Matrix4::getView(position, direction.toForward(), direction.toUp());
}

Matrix4 DirectionalLight::getProjectionMatrix() const {
    auto horizontalRange = _tHorizontalRange->getValue();
    auto verticalRange = _tVerticalRange->getValue();
    auto depthRange = _tDepthRange->getValue();
    return Matrix4::getOrtho(horizontalRange, verticalRange, depthRange);
}

void DirectionalLight::update() {
    cameraUbo.setBinding(CAMERA_BUFFER_BINDING);
    lightUbo.setBinding(LIGHT_BUFFER_BINDING);
    updateBuffers();
}

void DirectionalLight::updateBuffers() {
    dbgEnter("Update light buffers");

    auto lightPos = getPosition();
    auto lightDir = getDirection().toForward();
    auto viewMatrix = getViewMatrix();
    auto projMatrix = getProjectionMatrix();
    auto clipRange = _tDepthRange->getValue();

    auto matrixSize = 16 * sizeof(float);

    // update camera buffer
    //glBindBuffer(GL_UNIFORM_BUFFER, cameraUbo.resource);
    //glBufferData(GL_UNIFORM_BUFFER, (16 * 3 + 4 + 2) * sizeof(float), nullptr, cameraUbo.mode);
    //auto p = (float*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    auto p = cameraUbo.mapData((16 * 3 + 4 + 2) * sizeof(float));
    memcpy(p, viewMatrix.transpose().m, matrixSize);
    memcpy(p + 16, projMatrix.transpose().m, matrixSize);
    memcpy(p + 16*2, (projMatrix * viewMatrix).transpose().m, matrixSize);
    p[16*3] = lightPos.x;
    p[16*3+1] = lightPos.y;
    p[16*3+2] = lightPos.z;
    p[16*3+3] = 0; // todo: put something here?
    p[16*3+4] = clipRange.x;
    p[16*3+5] = clipRange.y;
    cameraUbo.unmapData();

    // update light buffer
    auto lightMatrix = projMatrix * viewMatrix;
    p = lightUbo.mapData((16 + 4) * sizeof(float));
    memcpy(p, lightMatrix.transpose().m, matrixSize);
    p[16] = lightDir.x;
    p[16+1] = lightDir.y;
    p[16+2] = lightDir.z;
    p[16+3] = 0; // todo: put something here?
    lightUbo.unmapData();
    dbgExit();
}
