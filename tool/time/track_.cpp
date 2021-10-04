#include "time/track_.h"
#include "time/sync.h"
#include "vector.h"
#include "track.h"

using namespace monad;

template<>
float Track<float>::getValue(double rows) {
    return (float)sync_get_val(tracks[0], rows);
}

template<>
int Track<int>::getValue(double rows) {
    return (int)sync_get_val(tracks[0], rows);
}

template<>
vec2 Track<vec2>::getValue(double rows) {
    return {
            (float)sync_get_val(tracks[0], rows),
            (float)sync_get_val(tracks[1], rows)
    };
}

template<>
vec3 Track<vec3>::getValue(double rows) {
    return {
            (float)sync_get_val(tracks[0], rows),
            (float)sync_get_val(tracks[1], rows),
            (float)sync_get_val(tracks[2], rows)
    };
}

template<>
vec4 Track<vec4>::getValue(double rows) {
    return {
            (float)sync_get_val(tracks[0], rows),
            (float)sync_get_val(tracks[1], rows),
            (float)sync_get_val(tracks[2], rows),
            (float)sync_get_val(tracks[3], rows)
    };
}

template<>
Direction Track<Direction>::getValue(double rows) {
    return {
            (float)(sync_get_val(tracks[0], rows) * M_PI / 180),
            (float)(sync_get_val(tracks[1], rows) * M_PI / 180),
            (float)(sync_get_val(tracks[2], rows) * M_PI / 180)
    };
}

template<>
ForwardDirection Track<ForwardDirection>::getValue(double rows) {
    return {
            (float)(sync_get_val(tracks[0], rows) * M_PI / 180),
            (float)(sync_get_val(tracks[1], rows) * M_PI / 180)
    };
}
