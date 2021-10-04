#include "debug.h"
#include "camera.h"
#include "time/track_.h"
#include "defines.h"

using namespace monad;

Camera::Camera(Track<vec3> *tCameraPos, Track<ForwardDirection> *tGymbalDir,
               Track<float> *tArmLength, Track<Direction> *tCameraDir,
               Track<float> *tFov, Track<vec2> *tClipRange)
        : _tCameraPos(tCameraPos), _tGymbalDir(tGymbalDir),
          _tArmLength(tArmLength), _tCameraDir(tCameraDir),
          _tFov(tFov), _tClipRange(tClipRange), ubo(GL_DYNAMIC_DRAW) { }

vec3 Camera::_getPosition() const {
    auto camPos = _tCameraPos->getValue();
    auto gymbalDir = _tGymbalDir->getValue();
    auto armLength = _tArmLength->getValue();

    return camPos + gymbalDir.toForward() * armLength;
}

Direction Camera::_getDirection() const {
    auto camDir = _tCameraDir->getValue();
    auto gymbalDir = _tGymbalDir->getValue().toDirection();

    return camDir + gymbalDir;
}

float Camera::getFov() const {
    return _tFov->getValue();
}

Matrix4 Camera::getViewMatrix() const {
    auto position = getPosition();
    auto direction = getDirection();

    return Matrix4::getView(position, direction.toForward(), direction.toUp());
}

Matrix4 Camera::getProjectionMatrix(float aspectRatio) const {
    auto clipRange = _tClipRange->getValue();
    return Matrix4::getPerspective(getFov(), aspectRatio, clipRange.x, clipRange.y);
}

void Camera::update(float aspectRatio) {
    _update();
    ubo.setBinding(CAMERA_BUFFER_BINDING);
    updateBuffer(aspectRatio);
}

void Camera::updateBuffer(float aspectRatio) {
    dbgEnter("Update camera buffer");
    auto camPos = getPosition();
    auto viewMatrix = getViewMatrix();
    auto projMatrix = getProjectionMatrix(aspectRatio);
    auto clipRange = _tClipRange->getValue();

    auto matrixSize = 16 * sizeof(float);

    //glBindBuffer(GL_UNIFORM_BUFFER, ubo.resource);
    //glBufferData(GL_UNIFORM_BUFFER, (16 * 3 + 4 + 2) * sizeof(float), nullptr, ubo.mode);
    //auto p = (float*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    auto p = ubo.mapData((16 * 3 + 4 + 2) * sizeof(float));
    memcpy(p, viewMatrix.transpose().m, matrixSize);
    memcpy(p + 16, projMatrix.transpose().m, matrixSize);
    memcpy(p + 16*2, lastFrameMatrix.transpose().m, matrixSize);
    p[16*3] = camPos.x;
    p[16*3+1] = camPos.y;
    p[16*3+2] = camPos.z;
    p[16*3+3] = getFov();
    p[16*3+4] = clipRange.x;
    p[16*3+5] = clipRange.y;
    ubo.unmapData();
    dbgExit();
}

void Camera::swap(float aspectRatio) {
    lastFrameMatrix = getProjectionMatrix(aspectRatio) * getViewMatrix();
}

void Camera::lock() {
    _isLocked = true;
}

void Camera::unlock() {
    _isLocked = false;
}
