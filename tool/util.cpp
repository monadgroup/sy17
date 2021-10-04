#include "util.h"
#include <cstdlib>

#include <stdio.h>

float lerpf(float a, float b, float f) {
    return a + (b - a) * f;
}

double lerp(double a, double b, double f) {
    return a + (b - a) * f;
}

float random(float min, float max) {
    auto r = rand();
    //printf("random=%i\n", r);
    auto randVal = ((float)r / RAND_MAX);

    return randVal * (max - min) + min;
}
