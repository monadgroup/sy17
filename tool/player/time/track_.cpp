#include "time/track_.h"
#include "vector.h"

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
    tracks[0] = manager->getTrack(name);
    tracks[1] = manager->getTrack(name);
}

template<>
Track<vec3>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(3) {
    init();
    tracks[0] = manager->getTrack(name);
    tracks[1] = manager->getTrack(name);
    tracks[2] = manager->getTrack(name);
}

template<>
Track<vec4>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(4) {
    init();
    tracks[0] = manager->getTrack(name);
    tracks[1] = manager->getTrack(name);
    tracks[2] = manager->getTrack(name);
    tracks[3] = manager->getTrack(name);
}

template<>
Track<Direction>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(3) {
    init();
    tracks[0] = manager->getTrack(name);
    tracks[1] = manager->getTrack(name);
    tracks[2] = manager->getTrack(name);
}

template<>
Track<ForwardDirection>::Track(TimeManager *manager, const char *name) : manager(manager), trackCount(2) {
    init();
    tracks[0] = manager->getTrack(name);
    tracks[1] = manager->getTrack(name);
}
