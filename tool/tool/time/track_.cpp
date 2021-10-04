#include "time/track_.h"
#include "time/sync.h"
#include "vector.h"
#include <string>

using namespace monad;

template<>
Track<float>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(1) {
    init();
    tracks[0] = manager->getTrack(name);
}

template<>
Track<int>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(1) {
    init();
    tracks[0] = manager->getTrack(name);
}

template<>
Track<vec2>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(2) {
    init();
    auto rname = std::string(name);
    tracks[0] = manager->getTrack((rname + ".x").c_str());
    tracks[1] = manager->getTrack((rname + ".y").c_str());
}

template<>
Track<vec3>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(3) {
    init();
    auto rname = std::string(name);
    tracks[0] = manager->getTrack((rname + ".x").c_str());
    tracks[1] = manager->getTrack((rname + ".y").c_str());
    tracks[2] = manager->getTrack((rname + ".z").c_str());
}

template<>
Track<vec4>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(4) {
    init();
    auto rname = std::string(name);
    tracks[0] = manager->getTrack((rname + ".x").c_str());
    tracks[1] = manager->getTrack((rname + ".y").c_str());
    tracks[2] = manager->getTrack((rname + ".z").c_str());
    tracks[3] = manager->getTrack((rname + ".w").c_str());
}

template<>
Track<Direction>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(3) {
    init();
    auto rname = std::string(name);
    tracks[0] = manager->getTrack((rname + ".pitch").c_str());
    tracks[1] = manager->getTrack((rname + ".yaw").c_str());
    tracks[2] = manager->getTrack((rname + ".roll").c_str());
}

template<>
Track<ForwardDirection>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(2) {
    init();
    auto rname = std::string(name);
    tracks[0] = manager->getTrack((rname + ".pitch").c_str());
    tracks[1] = manager->getTrack((rname + ".yaw").c_str());
}
