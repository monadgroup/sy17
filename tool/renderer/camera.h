#pragma once

#include "ubo.h"
#include "vector.h"

namespace monad {

    template<class T> class Track;

    class Camera {
        Track<vec3> *_tCameraPos;
        Track<ForwardDirection> *_tGymbalDir;
        Track<float> *_tArmLength;
        Track<Direction> *_tCameraDir;
        Track<float> *_tFov;
        Track<vec2> *_tClipRange;

        bool _isLocked = false;

        void _update();
        vec3 _getPosition() const;
        Direction _getDirection() const;

        Matrix4 lastFrameMatrix;

    public:
        Ubo ubo;

        Camera(Track<vec3> *tCameraPos, Track<ForwardDirection> *tGymbalDir,
               Track<float> *tArmLength, Track<Direction> *tCameraDir,
               Track<float> *tFov, Track<vec2> *tClipRange);

        vec3 getPosition() const;
        Direction getDirection() const;
        float getFov() const;

        Matrix4 getViewMatrix() const;
        Matrix4 getProjectionMatrix(float aspectRatio) const;

        void update(float aspectRatio);
        void updateBuffer(float aspectRatio);
        void swap(float aspectRatio);

        void lock();
        void unlock();
    };

}
