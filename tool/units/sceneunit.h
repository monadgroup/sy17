#pragma once
#include "def.h"
#include "directionallightunit.h"
#include "ssaounit.h"
#include "renderer/ubo.h"

namespace monad {

    class Context;
    class Texture;
    class Program;
    class IRenderable;
    template<class T> class Uniform;
    template<class T> class Track;
    class Camera;
    class Timer;

    class SceneUnit {
        Context *context;
        Timer *sceneTimer;
        Timer *shadeTimer;

        SHARED_PTR(Camera) camera;

        Ubo ubo;

        Framebuffer *renderFb;
        Framebuffer *shadeFb;

        Texture *posMaterialTex;
        Texture *normalTex;
        Texture *localPosTex;
        Texture *velocityTex;
        Texture *depthTex;

        Texture *colorTex;
        Texture *highlightsTex;

        DirectionalLightUnit directionalLight;
        SsaoUnit ssao;

    public:
        static ivec2 lightResolution;

        explicit SceneUnit(Context *context);

        void prepareProgram(SHARED_PTR(Program) program);
        void setCamera(SHARED_PTR(Camera) camera);
        void setLight(SHARED_PTR(DirectionalLight) light);
        void render(const SHARED_PTR(Program) program, IRenderable *renderable);

        Texture *getColorTexture() { return colorTex; }
        Texture *getHighlightsTexture() { return highlightsTex; }
        Texture *getPosTexture() { return posMaterialTex; }
        Texture *getNormalTexture() { return normalTex; }
        Texture *getLocalPosTexture() { return localPosTex; }
        Texture *getVelocityTexture() { return velocityTex; }
        Texture *getDepthTexture() { return depthTex; }
    };

}
