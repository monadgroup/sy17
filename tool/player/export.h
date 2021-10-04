#pragma once
#include "time/track.h"
#include <GL/gl.h>

namespace exports {

    extern const char shaderFragments[];
    extern const unsigned int shaderOffsets[];
    extern const unsigned char shaderCount;
    extern const unsigned char shaderLengths[];
    extern const GLenum shaderTypes[];
    extern const unsigned char shaderMaps[];

    extern const unsigned int trackRows[];
    extern const float trackValues[];
    extern const enum key_type trackTypes[];
    extern const unsigned char trackCount;
    extern const unsigned int trackOffsets[];
    extern const unsigned int trackSizes[];
    extern const unsigned char trackMaps[];

}
