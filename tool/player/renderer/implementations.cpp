#include "renderer/context.h"
#include "renderer/shader.h"
#include "time/timemanager.h"
#include "renderer/common.h"
#include "renderer/camera.h"
#include "renderer/timer.h"
#include "export.h"

using namespace monad;

Shader **loadedShaders = nullptr;
unsigned char nextShader = 0;

sync_track *loadedTracks = nullptr;
unsigned char nextTrack = 0;

void monad::init(demo::DemoWindow *window) {
    auto totalProgress = exports::shaderCount + 1.f;
    auto completedProgress = 0;

    // load shaders
    loadedShaders = new Shader*[exports::shaderCount];
    auto offsets = &exports::shaderOffsets[0];
    for (auto i = 0; i < exports::shaderCount; i++) {
        auto numFragments = exports::shaderLengths[i];
        auto shaderType = exports::shaderTypes[i];

        auto resource = glCreateShader(shaderType);
        auto fragments = new const char*[numFragments];
        for (auto x = 0; x < numFragments; x++) {
            fragments[x] = exports::shaderFragments + offsets[x];
        }
        offsets += numFragments;

        loadedShaders[i] = new Shader(resource, (int)numFragments, fragments);

        completedProgress++;
        window->displayProgress(0.5f * completedProgress / totalProgress);
    }

    // load tracks
    loadedTracks = new sync_track[exports::trackCount];
    for (auto i = 0; i < exports::trackCount; i++) {
        auto offset = exports::trackOffsets[i];
        auto rows = &exports::trackRows[offset];
        auto values = &exports::trackValues[offset];
        auto types = &exports::trackTypes[offset];

        auto numKeys = exports::trackSizes[i];

        struct track_key *keys = nullptr;

        if (numKeys > 0) {
            keys = new struct track_key[numKeys];
            for (auto x = 0; x < numKeys; x++) {
                keys[x].row = rows[x];
                keys[x].value = values[x];
                keys[x].type = types[x];
            }
        }

        loadedTracks[i].name = nullptr;
        loadedTracks[i].num_keys = numKeys;
        loadedTracks[i].keys = keys;
    }

    window->displayProgress(0.5f);
}

void monad::frameStart() { }

void monad::frameEnd() { }

SHARED_PTR(Shader) Context::getShader(const char *path, GLenum type) const {
    return loadedShaders[exports::shaderMaps[nextShader++]];
}

SHARED_PTR(Camera) Context::createCamera(Track<vec3> *tCameraPos, Track<ForwardDirection> *tGymbalDir,
                                         Track<float> *tArmLength, Track<Direction> *tCameraDir, Track<float> *tFov,
                                         Track<vec2> *tClipRange) const {
    return MAKE_SHARED(Camera, tCameraPos, tGymbalDir, tArmLength, tCameraDir, tFov, tClipRange);
}

Timer* Context::createTimer(const char *name) {
    return new Timer();
}

const sync_track *TimeManager::getTrack(const char *name) {
    auto track = &loadedTracks[exports::trackMaps[nextTrack++]];
    return track;
}
