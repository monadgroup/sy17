#pragma once
#include "renderer/scene.h"
#include "declarations.h"
#include "units/irenderable.h"

//#define DRAW_DEBUG_FRUSTUM

namespace monad {

    class MarsTestScene : public Scene, public IRenderable {
        static constexpr int obeliskRows = 2;
        static constexpr int obeliskColumns = 27;

        Timer *roverTrackTimer;
        Timer *terrainTimer;
        Timer *laserTimer;
        Timer *rocksTimer;
        Timer *obelisksTimer;
        Timer *citadelTimer;
        Timer *roverTimer;

        vec2 lastRoverPos = {0, 0};
        vec2 lastRoverDir = {0, 1};
        float lastRocketH = 0;

        bool roverEnabled;
        bool rocksEnabled;
        bool lasersEnabled;
        bool citadelEnabled;

        SHARED_PTR(Program) terrainProgram = nullptr;
        SHARED_PTR(Program) rockProgram = nullptr;
        SHARED_PTR(Program) obeliskProgram = nullptr;
        SHARED_PTR(Program) roverLightProgram = nullptr;
        SHARED_PTR(Program) roverProgram = nullptr;
        SHARED_PTR(Program) rocketProgram = nullptr;
        SHARED_PTR(Program) shadeProgram = nullptr;
        SHARED_PTR(Program) laserProgram = nullptr;
        SHARED_PTR(Program) roverTracksProgram = nullptr;
#ifdef DEBUG
        SHARED_PTR(Program) frustumProgram = nullptr;
#endif

        SHARED_PTR(Camera) camera1 = nullptr;
        SHARED_PTR(Camera) camera2 = nullptr;
        SHARED_PTR(DirectionalLight) light = nullptr;

        Ubo *terrainData = nullptr;
        Ubo *roverData = nullptr;

        RawObject *terrainPatch = nullptr;
        FeedbackObject *bigRocks = nullptr;
        RawObject *laser = nullptr;
#ifdef DEBUG
        RawObject *lightFrustum = nullptr;
#endif

        Object *roverBounds = nullptr;

        Object *obelisks[obeliskRows][obeliskColumns];
        Object *citadel;

        PingPong *roverTracks = nullptr;

        Uniform<float> *tesselMode = nullptr;
        Uniform<vec2> *obeliskCoord = nullptr;
        Uniform<float> *bloomThreshold = nullptr;
        Uniform<float> *obeliskPower = nullptr;
        Uniform<float> *obeliskFly = nullptr;
        Uniform<float> *obeliskWobble = nullptr;
        Uniform<float> *obeliskAmount = nullptr;
        Uniform<float> *obeliskMode = nullptr;
        Uniform<float> *obeliskDirtiness = nullptr;
        Uniform<float> *citadelAmount = nullptr;
        Uniform<float> *citadelMode = nullptr;
        Uniform<float> *guardAmount = nullptr;
        Uniform<float> *guardMode = nullptr;
        Uniform<float> *shipAmount = nullptr;
        Uniform<float> *shipRotateSpeed = nullptr;
        Uniform<float> *rocketAmount = nullptr;
        Uniform<float> *rocketHeight = nullptr;
        Uniform<float> *lastRocketHeight = nullptr;
        Uniform<float> *craterSpread = nullptr;
        Uniform<vec2> *terrainCenter = nullptr;
        Uniform<vec2> *rockCenter = nullptr;
        Uniform<float> *laserProgress1 = nullptr;
        Uniform<float> *laserProgress2 = nullptr;
        Uniform<float> *groundWidth = nullptr;
        Uniform<float> *nightAmount = nullptr;

        Uniform<vec2> *rocketViewportSize = nullptr;

        Uniform<int> *trackFeedbackSampler = nullptr;
        Uniform<int> *trackShadeSampler = nullptr;

        Track<float> *tesselModeTrack = nullptr;
        Track<float> *cameraTrack = nullptr;
        Track<vec2> *roverPosTrack = nullptr;
        Track<float> *obeliskPowerTrack = nullptr;
        Track<float> *obeliskFlyTrack = nullptr;
        Track<float> *obeliskWobbleTrack = nullptr;
        Track<float> *obeliskAmountTrack = nullptr;
        Track<float> *obeliskModeTrack = nullptr;
        Track<float> *bloomThresholdTrack = nullptr;
        Track<vec2> *craterPosTrack = nullptr;
        Track<float> *craterSizeTrack = nullptr;
        Track<float> *craterFalloffTrack = nullptr;
        Track<float> *rocketHeightTrack = nullptr;
        Track<float> *obeliskDirtinessTrack = nullptr;
        Track<float> *citadelModeTrack = nullptr;
        Track<float> *citadelAmountTrack = nullptr;
        Track<float> *guardModeTrack = nullptr;
        Track<float> *guardAmountTrack = nullptr;
        Track<float> *shipAmountTrack = nullptr;
        Track<float> *shipRotateSpeedTrack = nullptr;
        Track<float> *rocketAmountTrack = nullptr;
        Track<vec2> *terrainCenterTrack = nullptr;
        Track<int> *terrainFollowTrack = nullptr;
        Track<vec2> *rockCenterTrack = nullptr;
        Track<float> *craterSpreadTrack = nullptr;
        Track<float> *laserProgressTrack = nullptr;
        Track<float> *groundWidthTrack = nullptr;
        Track<float> *nightAmountTrack = nullptr;

        Track<int> *roverEnabledTrack = nullptr;
        Track<int> *rocksEnabledTrack = nullptr;
        Track<int> *lasersEnabledTrack = nullptr;
        Track<int> *citadelEnabledTrack = nullptr;

    public:
        MarsTestScene() = default;

        void render(RenderType type, const IFramebuffer *target, ivec4 viewport) override;

        void init(demo::DemoWindow *window) override;
        void enter() override;
        void frame() override;
        void leave() override;
    };

}
