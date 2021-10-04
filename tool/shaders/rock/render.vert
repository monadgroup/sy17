#include "../common/materials.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/mapNormal.glsl"
#include "../marsHeightmap.glsl"

layout(location=0) in vec3 inPos;
layout(location=1) in float inRockId;

out vec3 vPosition;
out float vMaterial;

uniform vec2 patchRepeats;
uniform vec2 patchSize;
uniform vec2 followPos;

void main() {
    vec2 patchPos = vec2(patchSize.x * mod(gl_InstanceID, patchRepeats.x), patchSize.y * floor(gl_InstanceID / patchRepeats.x));

    vec2 rockPos = vec2(noise(gl_InstanceID + inRockId + 593), noise(gl_InstanceID + inRockId + 47)) * patchSize;
    vec2 relativeRockPos = mod(rockPos - followPos.xy, patchSize) - patchSize/2;
    rockPos = patchPos + relativeRockPos + followPos.xy;

    // make rock fit heightmap normal
    vec3 tn = marsHeightmapNormal(rockPos);
    vec3 v = cross(vec3(0, 1, 0), tn);
    float c = dot(vec3(0, 1, 0), tn);
    mat3 vx = mat3(vec3(0, v.z, -v.y), vec3(-v.z, 0, v.x), vec3(v.y, -v.x, 0));
    mat3 r = mat3(1) + vx + vx*vx / (1 + c);

    vec3 offsetPos = vec3(
        rockPos.x,
        marsHeightmap(rockPos) - 0.09,
        rockPos.y
    );
    vPosition = offsetPos + r * inPos;
    vMaterial = MATERIAL_ROCK - 0.5;
}
