#pragma once

#include "gl.h"
#include "list.h"
#include "vector.h"
#include "def.h"
#include "shadertypes.h"
#include "texture.h"
#include "directionallight.h"
#include "demo/demowindow.h"
#include "framebuffer.h"
#include "units/irenderable.h"

namespace monad {

    template<class T> class Track;
    class TimeManager;

    class Mesh;
    class Shader;
    class Scene;
    class Program;
    class Object;
    class RawObject;
    class FeedbackObject;
    class Camera;
    class DirectionalLight;
    class Timer;

    class NoiseGenerator;
    class DiamondSquareGenerator;
    class RockGenerator;

    class SceneUnit;
    class PostUnit;

    class Context {
        static constexpr int numSceneTextures = 2;

        TimeManager *manager;

        SHARED_PTR(FragmentShader) rasterFragmentShader;
        SHARED_PTR(VertexShader) rasterVertexShader;
        SHARED_PTR(VertexShader) postVertexShader;
        SHARED_PTR(Program) objectProgram;

        ivec2 screenSize;
        ivec2 viewSize;
        ivec2 viewOffset;

        GLuint objectVao;
        RawObject *postObject;

        ScreenFramebuffer screenFramebuffer = ScreenFramebuffer();

        NoiseGenerator *noiseGenerator;
        DiamondSquareGenerator *diamondSquareGenerator;
        Texture *marsTerrain;

        SceneUnit *sceneUnit;
        PostUnit *postUnit;

    public:
        // interface for player
        Context(TimeManager *manager, demo::DemoWindow *window, ivec2 size);
        void renderFrame(Scene &scene, const ivec2 &screenSize);

        ivec2 getScreenSize() { return screenSize; }
        ivec2 getViewSize() { return viewSize; }
        ivec2 getViewOffset() { return viewOffset; }
        ivec4 getViewport() { return { 0, 0, viewSize.x, viewSize.y }; }
        ivec4 getScreenport() { return { viewOffset.x, viewOffset.y, viewSize.x, viewSize.y }; }
        float getViewAspect() { return (float)viewSize.x / viewSize.y; }

        // shader+program creation functions
        SHARED_PTR(Shader) getShader(const char *path, GLenum type) const;
        SHARED_PTR(VertexShader) getVertexShader(const char *path) const;
        SHARED_PTR(TessControlShader) getTessControlShader(const char *path) const;
        SHARED_PTR(TessEvaluationShader) getTessEvaluationShader(const char *path) const;
        SHARED_PTR(GeometryShader) getGeometryShader(const char *path) const;
        SHARED_PTR(FragmentShader) getFragmentShader(const char *path) const;
        SHARED_PTR(ComputeShader) getComputeShader(const char *path) const;

        SHARED_PTR(FragmentShader) getRasterFragmentShader() const { return rasterFragmentShader; }
        SHARED_PTR(VertexShader) getRasterVertexShader() const { return rasterVertexShader; }
        SHARED_PTR(VertexShader) getPostVertexShader() const { return postVertexShader; }

        SHARED_PTR(Program) createProgram() const;
        SHARED_PTR(Program) createProgram(SHARED_PTR(VertexShader) vertexShader, SHARED_PTR(FragmentShader) fragmentShader) const;
        SHARED_PTR(Program) createPostProgram(SHARED_PTR(FragmentShader) fragmentShader) const;

        SHARED_PTR(Program) getObjectProgram() const { return objectProgram; }

        // procedural functions
        NoiseGenerator *getNoiseGenerator() const { return noiseGenerator; }
        DiamondSquareGenerator *getDiamondSquareGenerator() const { return diamondSquareGenerator; }
        Texture *getMarsTerrain() const { return marsTerrain; }

        // camera creation functions
        SHARED_PTR(Camera) createCamera(Track<vec3> *tCameraPos, Track<ForwardDirection> *tGymbalDir,
                                        Track<float> *tArmLength, Track<Direction> *tCameraDir,
                                        Track<float> *tFov, Track<vec2> *tClipRange) const;

        // light creation functions
        SHARED_PTR(DirectionalLight) createDirectionalLight(Track<vec3> *tLightPos,
                                                            Track<ForwardDirection> *tLightDir,
                                                            Track<vec2> *tHorizontalRange, Track<vec2> *tVerticalRange,
                                                            Track<vec2> *tDepthRange);

        // buffer/object creation functions
        RawObject *createRawObject(GLenum usageMode = GL_STATIC_DRAW, GLenum renderMode = GL_PATCHES) const;
        Object *createObject(Mesh *mesh, GLenum usageMode = GL_STATIC_DRAW, GLenum renderMode = GL_TRIANGLES) const;
        FeedbackObject *createFeedbackObject(int bufferSize, RawObject *object) const;

        // framebuffer control functions
        Framebuffer *createFramebuffer(int colorCount, const TextureImage *colors, const TextureImage *depth = nullptr, const TextureImage *stencil = nullptr) const;
        const ScreenFramebuffer *getScreenFramebuffer() const { return &screenFramebuffer; }

        // pipeline functions
        SceneUnit *getSceneUnit() const { return sceneUnit; }
        PostUnit *getPostUnit() const { return postUnit; }
        void runPipeline(const SHARED_PTR(Program) shadeProgram, IRenderable *renderable);

        // performance functions
        Timer *createTimer(const char *name);

        // track functions
        template<class T>
        Track<T> *getTrack(const char *name) const {
            return new Track<T>(manager, name);
        }

        float getTime();

        // rendering functions
        RawObject *getQuad() { return postObject; }
    };

}
