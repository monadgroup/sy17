#include "../common/math.glsl"
#include "../common/noise.glsl"

in vec2 texCoords;
out vec3 outTex;

uniform sampler2D sColor;

uniform int ghostCount;
uniform float ghostDispersal;
uniform float haloWidth;
uniform vec2 viewportSize;

vec3 lensColor(float d) {
    d = 1 - d*5;
    return saturate(vec3(
        noise(vec3(d, 0, 0)),
        noise(vec3(0, d, 0)),
        noise(vec3(0, 0, d))
    ));
}

void main() {
    vec2 flippedCoord = 1 - texCoords;
    vec2 texelSize = 1 / viewportSize;

    vec2 ghostVec = (0.5 - flippedCoord) * ghostDispersal;
    vec3 result = vec3(0);
    for (int i = 0; i < ghostCount; i++) {
        vec2 offset = fract(flippedCoord + ghostVec * float(i));

        float weight = length(0.5 - offset) / length(vec2(0.5));
        weight = pow(1 - weight, 10);

        result += texture(sColor, offset).rgb * weight * lensColor(float(i)) * mix(0.2, 1., fbm(offset*20));
    }
    result = pow(result, vec3(2));

    vec2 haloVec = normalize(ghostVec) * haloWidth;
    float weight = length(0.5 - fract(flippedCoord + haloVec)) / length(vec2(0.5));
    weight = pow(1 - weight, 5);
    result += texture(sColor, flippedCoord + haloVec).rgb * weight * 0.2;

    outTex = result;
}

