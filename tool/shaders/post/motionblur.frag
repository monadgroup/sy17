#include "../common/camera_data.glsl"
#include "../common/getRay.glsl"
#include "../common/linearizeDepth.glsl"
#include "../common/unproject.glsl"

in vec2 texCoords;
out vec3 outTex;

uniform sampler2D sColor;
uniform sampler2D sDepth;
uniform sampler2D sVelocity;

const int numSamples = 10;

void main() {
    vec3 projectCoords = vec3(texCoords, texture(sDepth, texCoords).r);
    vec3 pos = unproject(projectCoords, clipRange, viewMatrix, projectionMatrix);

    vec3 inVel = texture(sVelocity, texCoords).xyz;
    vec4 oldCoordQ = lastFrameMatrix * vec4(pos - inVel, 1);
    vec2 oldCoord = (oldCoordQ.xy / oldCoordQ.w) * 0.5 + 0.5;

    vec2 velocity = (texCoords - oldCoord) / numSamples;

    vec3 accumulate = vec3(0);
    vec2 currentCoord = texCoords;
    for (int i = 0; i < numSamples; i++) {
        accumulate += texture(sColor, currentCoord).rgb;
        currentCoord += velocity;
    }

    outTex = max(vec3(0), accumulate / numSamples);
}
