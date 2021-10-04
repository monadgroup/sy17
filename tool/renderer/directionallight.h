#pragma once
#include "ubo.h"
#include "vector.h"

namespace monad {

    template<class T> class Track;
    class Texture;

    class DirectionalLight {
        Track<vec3> *_tLightPos;
        Track<ForwardDirection> *_tLightDir;
        Track<vec2> *_tHorizontalRange;
        Track<vec2> *_tVerticalRange;
        Track<vec2> *_tDepthRange;

    public:
        Ubo cameraUbo;
        Ubo lightUbo;

        DirectionalLight(Track<vec3> *tLightPos, Track<ForwardDirection> *tLightDir, Track<vec2> *tHorizontalRange, Track<vec2> *tVerticalRange, Track<vec2> *tDepthRange);

        vec3 getPosition() const;
        ForwardDirection getDirection() const;

        Matrix4 getViewMatrix() const;
        Matrix4 getProjectionMatrix() const;

        void update();
        void updateBuffers();
    };

}
