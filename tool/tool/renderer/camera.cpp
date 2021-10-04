#include "renderer/camera.h"
#include "ui/toolwindow.h"
#include "time/track_.h"
#include "debug.h"

using namespace monad;

void Camera::_update() {
    monad::lastCamera = this;
}

vec3 Camera::getPosition() const {
    return _isLocked ? monad::lockCameraPos : _getPosition();
}

Direction Camera::getDirection() const {
    return _isLocked ? monad::lockCameraDir : _getDirection();
}
