#include "renderer/camera.h"

using namespace monad;

void Camera::_update() {

}

vec3 Camera::getPosition() const {
    return _getPosition();
}

Direction Camera::getDirection() const {
    return _getDirection();
}
