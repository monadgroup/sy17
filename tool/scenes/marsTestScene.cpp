#include <imgui/imgui.h>
#include "marsTestScene.h"
#include "includes.h"

using namespace monad;

static float quadWidth = 1;
static float quadHeight = 1;
static float patchRows = 25;
static float patchColumns = 25;

static const vec2 rockPatchRepeats = {1, 1};
static const vec2 rockPatchSize = {5, 5};
static const ivec2 roverTrackRes = {4096, 4096};
static const vec2 roverTrackSize = {2.5, 2.5};

static bool showDebugFrustum;

void MarsTestScene::init(demo::DemoWindow *window) {
    dbgEnter("MarsTestScene init");
    roverTrackTimer = context->createTimer("RoverTracks");
    terrainTimer = context->createTimer("Terrain");
    laserTimer = context->createTimer("Lasers");
    rocksTimer = context->createTimer("Rocks");
    obelisksTimer = context->createTimer("Obelisks");
    citadelTimer = context->createTimer("Citadel");
    roverTimer = context->createTimer("Rover");

    auto terrainHeightmap = context->getMarsTerrain();

    // progress 0.5->0.6 for creating objects

    // terrain object
    static const float terrainPatchData[] = {
            0.f, 0.f, 0.f,
            1.f, 0.f, 0.f,
            1.f, 0.f, 1.f,
            0.f, 0.f, 1.f,
    };
    terrainPatch = context->createRawObject();
    terrainPatch->setData(4, sizeof(terrainPatchData), terrainPatchData);
    terrainPatch->attrib(3, 3, 0);
    window->displayProgress(0.525);

    // terrain UBO
    terrainData = new Ubo(GL_DYNAMIC_DRAW);
    roverData = new Ubo(GL_DYNAMIC_DRAW);

    // rock object
    static const float rockPatchData[] = { 0, 0, 0 };
    auto rockPatch = context->createRawObject();
    rockPatch->setData(1, sizeof(rockPatchData), rockPatchData);
    window->displayProgress(0.55);

    // laser object
    static const float laserData[] = {
        -0.5f, -0.5f, 0.f,
         0.5f, -0.5f, 0.f,
         0.5f,  0.5f, 0.f,
        -0.5f,  0.5f, 0.f,
    };
    laser = context->createRawObject(GL_STATIC_DRAW, GL_QUADS);
    laser->setData(4, sizeof(laserData), laserData);
    laser->attrib(3, 3, 0);
    window->displayProgress(0.575);

    // light frustum debug
#ifdef DEBUG
    lightFrustum = context->createRawObject(GL_STATIC_DRAW, GL_LINES);
    static const float frustumData[] = {
            -1,  1,  1,
            1,  1,  1,

            1,  1,  1,
            1, -1,  1,

            1, -1,  1,
            -1, -1,  1,

            -1, -1,  1,
            -1,  1,  1,


            -1,  1, -1,
            1,  1, -1,

            1,  1, -1,
            1, -1, -1,

            1, -1, -1,
            -1, -1, -1,

            -1, -1, -1,
            -1,  1, -1,


            -1,  1,  1,
            -1,  1, -1,

            1,  1,  1,
            1,  1, -1,

            1, -1,  1,
            1, -1, -1,

            -1, -1,  1,
            -1, -1, -1
    };
    lightFrustum->setData(24, sizeof(frustumData), frustumData);
    lightFrustum->attrib(3, 3, 0);
#endif

    // rover bounding object
    auto roverBoundMesh = new Mesh();
    gen::box(roverBoundMesh, MATERIAL_WHITE, {0.2, 0.2, 0.2});
    roverBounds = context->createObject(roverBoundMesh);
    window->displayProgress(0.6);

    // progress 0.6->0.8 for obelisks

    // obelisks
    auto obeliskGenerator = ObeliskGenerator(20, {20, 80, 20}, 0.005, MATERIAL_OBELISK_PILLARS, MATERIAL_OBELISK);
    window->displayProgress(0.6f + 0.2f/9);
    vec2 obeliskGridSize = {1, 0.2};

    // assign obelisk positions
    for (auto &obelisk : obelisks) {
        for (auto &x : obelisk) {
            auto obeliskMesh = new Mesh();
            obeliskGenerator.generate(obeliskMesh, Matrix4::getIdentity());
            x = context->createObject(obeliskMesh);
        }
    }
    window->displayProgress(0.6f + 0.2f/9*2);

    auto citadelGenerator = ObeliskGenerator(4, {80, 310, 80}, 0.03, MATERIAL_OBELISK_CITADEL, MATERIAL_OBELISK);
    window->displayProgress(0.6f + 0.2f/9*3);
    auto citadelMesh = new Mesh();
    citadelGenerator.generate(citadelMesh, Matrix4::move({0, -5, 0}));
    window->displayProgress(0.6f + 0.2f/9*4);

    auto pillarGenerator = ObeliskGenerator(10, {20, 600, 20}, 0.025, MATERIAL_OBELISK_CITADEL, MATERIAL_OBELISK);
    window->displayProgress(0.6f + 0.2f/9*5);
    for (auto x = 0; x < 2; x++) {
        for (auto y = 0; y < 2; y++) {
            auto xPos = (x*2 - 1) * 0.94f;
            auto yPos = (y*2 - 1) * 0.94f;
            pillarGenerator.generate(citadelMesh, Matrix4::move({xPos, 3.85, yPos}));
        }
    }
    window->displayProgress(0.6f + 0.2f/9*6);

    auto supportTransform = Matrix4::move({0, 0, 6.1});
    auto supportPillarTransform = Matrix4::move({0, 0, 4.2});
    auto supportRot = Matrix4::move({0, 3, 0}) * Matrix4::rotateX(-0.45);
    auto guardTransform = Matrix4::move({0, 0, 10}) * Matrix4::rotateX(0.2);

    auto guardTowerGenerator = ObeliskGenerator(10, {40, 160, 40}, 0.03, MATERIAL_OBELISK_GUARDS, MATERIAL_OBELISK);
    window->displayProgress(0.6f + 0.2f/9*7);
    for (auto i = 0; i < 8; i++) {
        auto supportItmTransform = Matrix4::rotateY(i * M_PI/4 - M_PI/8);
        guardTowerGenerator.generate(citadelMesh, supportItmTransform * guardTransform);

        gen::box(citadelMesh, MATERIAL_OBELISK - 0.5, {0.2, 0.5, 0.5}, supportItmTransform * supportTransform);

        auto pillarItmTransform = supportItmTransform * supportRot;
        gen::box(citadelMesh, MATERIAL_OBELISK_SUPPORTS - 0.5, {0.1, 0.1, 3.1}, pillarItmTransform * supportPillarTransform);

        for (auto x = 0; x < 12; x++) {
            auto boxTransform = pillarItmTransform * Matrix4::move({0, 0, -2}) * supportPillarTransform * Matrix4::move({0, 0, x * 0.4f - 0.4f}) * Matrix4::shearY(0, -0.5);
            gen::box(citadelMesh, MATERIAL_OBELISK - 0.5, {0.12, 0.12, 0.15}, boxTransform);
        }
    }
    window->displayProgress(0.6f + 0.2f/9*8);

    gen::box(citadelMesh, MATERIAL_OBELISK - 0.5, {3.5, 0.01, 0.2}, Matrix4::move({3.8, 0.57, 0}));
    citadel = context->createObject(citadelMesh);
    window->displayProgress(0.8);

    // common shaders
    auto normalGeom = context->getGeometryShader("getNormal.geom");

    // progress 0.8->0.9 for rock gen

    // generate rocks
    const char *genVaryingNames[] = { "vPosition", "vRockId" };
    auto rockGenProgram = context->createProgram();
    rockGenProgram->attachShader(context->getVertexShader("rock/generator.vert"));
    rockGenProgram->attachShader(context->getTessControlShader("rock/generator.tesc"));
    rockGenProgram->attachShader(context->getTessEvaluationShader("rock/generator.tese"));
    glTransformFeedbackVaryings(rockGenProgram->resource, 2, genVaryingNames, GL_INTERLEAVED_ATTRIBS);
    rockGenProgram->complete();
    window->displayProgress(0.85);

    glPatchParameteri(GL_PATCH_VERTICES, 1);
    auto bigRockCount = 20;
    auto mediumRockCount = 1000;
    auto smallRockCount = 19000;

    auto rockSize = rockGenProgram->getUniform<float>("rockSize", 0);
    bigRocks = context->createFeedbackObject(4*64*64*10*(bigRockCount + mediumRockCount + smallRockCount)*sizeof(float), rockPatch);
    bigRocks->startRenderFeedback(rockGenProgram);
    rockSize->setVal(0.4);
    bigRocks->doRenderFeedback(bigRockCount);
    rockSize->setVal(0.1);
    bigRocks->doRenderFeedback(mediumRockCount);
    rockSize->setVal(0.04);
    bigRocks->doRenderFeedback(smallRockCount);
    bigRocks->endRenderFeedback();

    bigRocks->attrib(3, 4, 0);
    bigRocks->attrib(1, 4, 3);
    window->displayProgress(0.9);

    // progress 0.9->1 for creating programs

    // terrain generation
    auto terrainVert = context->getVertexShader("terrain/instanced.vert");
    auto terrainTesc = context->getTessControlShader("terrain/control.tesc");
    auto terrainTese = context->getTessEvaluationShader("terrain/evaluate.tese");
    auto terrainFrag = context->getFragmentShader("terrain/shade.frag");
    terrainProgram = context->createProgram(terrainVert, terrainFrag);
    terrainProgram->attachShader(terrainTesc);
    terrainProgram->attachShader(terrainTese);
    terrainProgram->complete();
    terrainProgram->setSampler("inHeightmap", terrainHeightmap);
    terrainProgram->getUniform<ivec2>("viewportSize", context->getViewSize());
    terrainProgram->getUniform<vec2>("patchRepeats", { patchRows, patchColumns });
    terrainProgram->getUniform<vec2>("patchSize", { quadWidth, quadHeight });
    terrainCenter = terrainProgram->getUniform<vec2>("followPos", {0, 0});
    tesselMode = terrainProgram->getUniform<float>("tesselMode", 0);
    window->displayProgress(0.9f + 0.1f/9);

    // rock rendering
    rockProgram = context->createProgram(context->getVertexShader("rock/render.vert"), context->getRasterFragmentShader());
    rockProgram->attachShader(normalGeom);
    rockProgram->complete();
    rockProgram->setSampler("inHeightmap", terrainHeightmap);
    rockProgram->getUniform<vec2>("patchRepeats", rockPatchRepeats);
    rockProgram->getUniform<vec2>("patchSize", rockPatchSize);
    rockCenter = rockProgram->getUniform<vec2>("followPos", {0, 0});
    window->displayProgress(0.9f + 0.1f/9*2);

    // obelisk rendering
    obeliskProgram = context->createProgram(context->getVertexShader("obelisk.vert"), context->getRasterFragmentShader());
    obeliskProgram->complete();
    obeliskProgram->setSampler("inHeightmap", terrainHeightmap);
    obeliskCoord = obeliskProgram->getUniform<vec2>("coord", {0, 0});
    obeliskPower = obeliskProgram->getUniform<float>("offsetAmount", 0);
    obeliskFly = obeliskProgram->getUniform<float>("flyAmount", 0);
    obeliskWobble = obeliskProgram->getUniform<float>("wobbleAmount", 0);
    window->displayProgress(0.9f + 0.1f/9*3);

    // rover rendering
    auto roverVert = context->getVertexShader("rover/bound.vert");
    auto roverFrag = context->getFragmentShader("rover/march.frag");
    roverProgram = context->createProgram(roverVert, roverFrag);
    roverProgram->complete();
    roverProgram->setSampler("inHeightmap", terrainHeightmap);
    roverProgram->getUniform<vec2>("windowSize", vec2(context->getViewSize()));

    roverLightProgram = context->createPostProgram(roverFrag);
    roverLightProgram->complete();
    roverLightProgram->setSampler("inHeightmap", terrainHeightmap);
    roverLightProgram->getUniform<vec2>("windowSize", vec2(SceneUnit::lightResolution));
    window->displayProgress(0.9f + 0.1f/9*4);

    // rover tracks rendering
    auto tracksFrag = context->getFragmentShader("rover/tracks.glsl");
    roverTracksProgram = context->createPostProgram(tracksFrag);
    roverTracksProgram->complete();
    roverTracksProgram->getUniform<vec2>("worldSize", roverTrackSize);
    roverTracksProgram->getUniform<vec2>("viewportSize", vec2(roverTrackRes));
    trackFeedbackSampler = roverTracksProgram->getUniform<int>("sLast", 0);
    window->displayProgress(0.9f + 0.1f/9*5);

    auto trackTex1 = new Texture("Rover track ping", GL_R16F, GL_RED, GL_FLOAT);
    trackTex1->addImage(roverTrackRes);
    auto trackTex2 = new Texture("Rover track pong", GL_R16F, GL_RED, GL_FLOAT);
    trackTex2->addImage(roverTrackRes);
    roverTracks = new PingPong(context, trackTex1, trackTex2);

    // rocket rendering
    auto rocketFrag = context->getFragmentShader("rocket/march.frag");
    rocketProgram = context->createPostProgram(rocketFrag);
    rocketProgram->complete();
    rocketViewportSize = rocketProgram->getUniform<vec2>("windowSize", {0, 0});
    rocketHeight = rocketProgram->getUniform<float>("rocketHeight", 0);
    lastRocketHeight = rocketProgram->getUniform<float>("lastRocketHeight", 0);
    shipRotateSpeed = rocketProgram->getUniform<float>("rotateSpeed", 0);
    window->displayProgress(0.9f + 0.1f/9*6);

    // laser rendering
    auto laserVert = context->getVertexShader("laser/laser.vert");
    laserProgram = context->createProgram(laserVert, context->getRasterFragmentShader());
    laserProgram->complete();
    laserProgram->getUniform<vec2>("obeliskGridSize", vec2(obeliskRows, obeliskColumns));
    laserProgress1 = laserProgram->getUniform<float>("laserProgress", 0);
    window->displayProgress(0.9f + 0.1f/9*7);

    // frustum debug
#ifdef DEBUG
    auto frustumVert = context->getVertexShader("debug/lightFrustum.vert");
    auto frustumFrag = context->getFragmentShader("debug/lightFrustum.frag");
    frustumProgram = context->createProgram(frustumVert, frustumFrag);
    frustumProgram->complete();
#endif

    // G-buffer shading
    auto shadeFrag = context->getFragmentShader("scenes/marsTest.frag");
    shadeProgram = context->createPostProgram(shadeFrag);
    shadeProgram->complete();
    shadeProgram->setSampler("inHeightmap", terrainHeightmap);
    shadeProgram->getUniform<vec2>("viewportSize", vec2(context->getViewSize()));
    shadeProgram->getUniform<vec2>("trackSize", roverTrackSize);
    bloomThreshold = shadeProgram->getUniform<float>("bloomThreshold", 0);
    trackShadeSampler = shadeProgram->getUniform<int>("sTrack", 0);
    obeliskAmount = shadeProgram->getUniform<float>("obeliskAmount", 0);
    obeliskMode = shadeProgram->getUniform<float>("obeliskMode", 0);
    obeliskDirtiness = shadeProgram->getUniform<float>("obeliskDirtiness", 0);
    citadelAmount = shadeProgram->getUniform<float>("citadelAmount", 0);
    citadelMode = shadeProgram->getUniform<float>("citadelMode", 0);
    guardAmount = shadeProgram->getUniform<float>("guardAmount", 0);
    guardMode = shadeProgram->getUniform<float>("guardMode", 0);
    shipAmount = shadeProgram->getUniform<float>("shipAmount", 0);
    rocketAmount = shadeProgram->getUniform<float>("rocketAmount", 0);
    craterSpread = shadeProgram->getUniform<float>("craterSpread", 0);
    laserProgress2 = shadeProgram->getUniform<float>("laserProgress", 0);
    groundWidth = shadeProgram->getUniform<float>("groundWidth", 0);
    nightAmount = shadeProgram->getUniform<float>("nightAmount", 0);
    context->getSceneUnit()->prepareProgram(shadeProgram);
    window->displayProgress(0.9f + 0.1f/9*8);

    camera1 = context->createCamera(
            context->getTrack<vec3>("camera1:pos"),
            context->getTrack<ForwardDirection>("camera1:gymbal"),
            context->getTrack<float>("camera1:length"),
            context->getTrack<Direction>("camera1:dir"),
            context->getTrack<float>("camera1:fov"),
            context->getTrack<vec2>("camera1:clipRange")
    );
    camera2 = context->createCamera(
            context->getTrack<vec3>("camera2:pos"),
            context->getTrack<ForwardDirection>("camera2:gymbal"),
            context->getTrack<float>("camera2:length"),
            context->getTrack<Direction>("camera2:dir"),
            context->getTrack<float>("camera2:fov"),
            context->getTrack<vec2>("camera2:clipRange")
    );

    light = context->createDirectionalLight(
            context->getTrack<vec3>("light:pos"),
            context->getTrack<ForwardDirection>("light:dir"),
            context->getTrack<vec2>("light:xRange"),
            context->getTrack<vec2>("light:yRange"),
            context->getTrack<vec2>("light:zRange")
    );

    cameraTrack = context->getTrack<float>("camera");
    roverPosTrack = context->getTrack<vec2>("rover:pos");
    obeliskPowerTrack = context->getTrack<float>("alien:power");
    obeliskFlyTrack = context->getTrack<float>("alien:fly");
    obeliskWobbleTrack = context->getTrack<float>("alien:wobble");
    obeliskAmountTrack = context->getTrack<float>("alien:obeliskAmount");
    obeliskModeTrack = context->getTrack<float>("alien:obeliskMode");
    bloomThresholdTrack = context->getTrack<float>("post:bloomThresh");

    craterPosTrack = context->getTrack<vec2>("alien:craterPos");
    craterSizeTrack = context->getTrack<float>("alien:craterSize");
    craterFalloffTrack = context->getTrack<float>("alien:craterFalloff");

    rocketHeightTrack = context->getTrack<float>("alien:rocketHeight");
    obeliskDirtinessTrack = context->getTrack<float>("alien:obeliskDirt");
    citadelModeTrack = context->getTrack<float>("alien:citadelMode");
    citadelAmountTrack = context->getTrack<float>("alien:citadelAmount");
    guardModeTrack = context->getTrack<float>("alien:guardMode");
    guardAmountTrack = context->getTrack<float>("alien:guardAmount");
    shipAmountTrack = context->getTrack<float>("alien:shipAmount");
    shipRotateSpeedTrack = context->getTrack<float>("alien:shipRotate");
    rocketAmountTrack = context->getTrack<float>("alien:rocketAmount");
    laserProgressTrack = context->getTrack<float>("alien:laserProgress");
    groundWidthTrack = context->getTrack<float>("alien:groundWidth");
    terrainCenterTrack = context->getTrack<vec2>("scene:terrainCenter");
    terrainFollowTrack = context->getTrack<int>("scene:terrainFollow");
    rockCenterTrack = context->getTrack<vec2>("scene:rockCenter");
    craterSpreadTrack = context->getTrack<float>("scene:craterSpread");
    nightAmountTrack = context->getTrack<float>("scene:nightAmount");

    roverEnabledTrack = context->getTrack<int>("scene:roverEnabled");
    rocksEnabledTrack = context->getTrack<int>("scene:rocksEnabled");
    lasersEnabledTrack = context->getTrack<int>("scene:lasersEnabled");
    citadelEnabledTrack = context->getTrack<int>("scene:citadelEnabled");
    tesselModeTrack = context->getTrack<float>("scene:tesselMode");

    window->displayProgress(1);

    dbgExit();
}

void MarsTestScene::enter() {
    dbgEnter("MarsTestScene enter");
    context->getSceneUnit()->setLight(light);
    terrainData->setBinding(TERRAIN_BUFFER_BINDING);
    roverData->setBinding(ROVER_BUFFER_BINDING);
    dbgExit();
}

void MarsTestScene::render(RenderType type, const IFramebuffer *target, ivec4 viewport) {
    dbgEnter("MarsTestScene render");

    target->bind(viewport);

    auto rockPatchCount = (int)(rockPatchRepeats.x * rockPatchRepeats.y);

    if (type != RenderType::LIGHTING) {
        dbgEnter("Render terrain");
        terrainTimer->start();
        //glPolygonMode(GL_FRONT, GL_LINE);
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        terrainPatch->render(terrainProgram, (int) (patchRows * patchColumns));
        //glPolygonMode(GL_FRONT, GL_FILL);
        terrainTimer->stop();
        dbgExit();

        dbgEnter("Render lasers");
        laserTimer->start();
        if (lasersEnabled) {
            glDisable(GL_CULL_FACE);
            laser->render(laserProgram, 2);
            glEnable(GL_CULL_FACE);
        }
        laserTimer->stop();
        dbgExit();

#ifdef DEBUG
        if (ImGui::Begin("Scene")) {
            ImGui::Checkbox("Render debug frustum", &showDebugFrustum);
        }
        ImGui::End();

        if (showDebugFrustum) {
            dbgEnter("Render debug frustum");
            lightFrustum->render(frustumProgram);
            dbgExit();
        }
#endif

        rocksTimer->start();
        if (rocksEnabled) {
            bigRocks->render(rockProgram, rockPatchCount);
            //smallRocks->render(rockProgram, rockPatchCount);
        }
        rocksTimer->stop();
    }

    dbgEnter("Render obelisks");
    if (type != RenderType::LIGHTING) obelisksTimer->start();
    for (auto y = 0; y < obeliskRows; y++) {
        for (auto x = 0; x < obeliskColumns; x++) {
            auto obelisk = obelisks[y][x];
            obeliskCoord->setVal(vec2(x, y));
            obelisk->render(obeliskProgram, 1);
        }
    }
    if (type != RenderType::LIGHTING) obelisksTimer->stop();
    dbgExit();

    dbgEnter("Render citadel");
    if (type != RenderType::LIGHTING) citadelTimer->start();
    if (citadelEnabled) {
        citadel->render(context->getObjectProgram(), 1);
        rocketViewportSize->setVal(vec2(viewport.z, viewport.w));
        context->getQuad()->render(rocketProgram, 1);
    }
    if (type != RenderType::LIGHTING) citadelTimer->stop();
    dbgExit();

    dbgEnter("Render rover");
    if (type != RenderType::LIGHTING) roverTimer->start();
    if (roverEnabled) {
        if (type == RenderType::LIGHTING) context->getQuad()->render(roverLightProgram);
        else roverBounds->render(roverProgram, 1);
    }
    if (type != RenderType::LIGHTING) roverTimer->stop();
    dbgExit();

    dbgExit();
}

void MarsTestScene::frame() {
    dbgEnter("MarsTestScene frame");

    auto pickCamera = fmodf(cameraTrack->getValue(), 2);
    auto selectCam = pickCamera < 1 ? camera1 : camera2;
    context->getSceneUnit()->setCamera(selectCam);

    obeliskPower->setVal(obeliskPowerTrack->getValue());
    obeliskFly->setVal(obeliskFlyTrack->getValue());
    obeliskWobble->setVal(obeliskWobbleTrack->getValue());
    obeliskAmount->setVal(obeliskAmountTrack->getValue());
    obeliskMode->setVal(obeliskModeTrack->getValue());
    bloomThreshold->setVal(bloomThresholdTrack->getValue());

    auto rocketH = rocketHeightTrack->getValue();
    rocketHeight->setVal(rocketH);
    lastRocketHeight->setVal(lastRocketH);
    lastRocketH = rocketH;

    obeliskDirtiness->setVal(obeliskDirtinessTrack->getValue());
    citadelAmount->setVal(citadelAmountTrack->getValue());
    citadelMode->setVal(citadelModeTrack->getValue());
    guardAmount->setVal(guardAmountTrack->getValue());
    guardMode->setVal(guardModeTrack->getValue());
    shipAmount->setVal(shipAmountTrack->getValue());
    rocketAmount->setVal(rocketAmountTrack->getValue());
    craterSpread->setVal(craterSpreadTrack->getValue());
    shipRotateSpeed->setVal(shipRotateSpeedTrack->getValue());
    groundWidth->setVal(groundWidthTrack->getValue());
    nightAmount->setVal(nightAmountTrack->getValue());
    tesselMode->setVal(tesselModeTrack->getValue());

    auto laserProgress = laserProgressTrack->getValue();
    laserProgress1->setVal(laserProgress);
    laserProgress2->setVal(laserProgress);

    if (terrainFollowTrack->getValue()) {
        auto camPos = selectCam->getPosition();
        terrainCenter->setVal({camPos.x, camPos.z});
    } else {
        terrainCenter->setVal(terrainCenterTrack->getValue());
    }

    rockCenter->setVal(rockCenterTrack->getValue());

    roverEnabled = (bool)roverEnabledTrack->getValue();
    rocksEnabled = (bool)rocksEnabledTrack->getValue();
    lasersEnabled = (bool)lasersEnabledTrack->getValue();
    citadelEnabled = (bool)citadelEnabledTrack->getValue();

    // update rover data
    auto roverPos = roverPosTrack->getValue();
    auto roverDelta = roverPos - lastRoverPos;
    lastRoverPos = roverPos;

    auto roverDir = roverDelta.unit();
    if (roverDir.x == 0 && roverDir.y == 0) roverDir = lastRoverDir;
    else {
        roverDir = (roverDir + lastRoverDir * 20).unit();
    }

    dbgEnter("Update rover buffer");
    auto p = roverData->mapData((2 + 2 + 2 + 2)*sizeof(float));
    p[0] = roverPos.x;
    p[1] = roverPos.y;
    p[2] = roverDelta.x;
    p[3] = roverDelta.y;
    p[4] = roverDir.x;
    p[5] = roverDir.y;
    p[6] = lastRoverDir.x;
    p[7] = lastRoverDir.y;
    roverData->unmapData();
    dbgExit();

    lastRoverDir = roverDir;

    // update terrain data
    dbgEnter("Update terrain buffer");
    auto craterPos = craterPosTrack->getValue();
    p = terrainData->mapData((2 + 1 + 1)*sizeof(float));
    p[0] = craterPos.x;
    p[1] = craterPos.y;
    p[2] = craterSizeTrack->getValue();
    p[3] = craterFalloffTrack->getValue();
    terrainData->unmapData();
    dbgExit();

    citadel->setTransform(Matrix4::move({craterPos.x, 0, craterPos.y}));

    // render rover tracks
    dbgEnter("Render rover tracks");
    roverTrackTimer->start();
    if (roverEnabled) {
        roverTracks->swapAndBind({0, 0, roverTrackRes.x, roverTrackRes.y}, trackFeedbackSampler);
        context->getQuad()->render(roverTracksProgram);
        trackShadeSampler->setVal(roverTracks->getWriteTex()->unit);
    }
    roverTrackTimer->stop();
    dbgExit();

    context->runPipeline(shadeProgram, this);

    // update camera swaps
    auto viewAspect = context->getViewAspect();
    camera1->swap(viewAspect);
    camera2->swap(viewAspect);

    dbgExit();
}

void MarsTestScene::leave() {

}
