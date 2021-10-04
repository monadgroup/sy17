#include "common/camera_data.glsl"
#include "common/linearizeDepth.glsl"

const int aoKernelSize = 64;

in vec2 texCoords;
out float aoTex;

uniform sampler2D sPosMaterial;
uniform sampler2D sNormal;
uniform sampler2D sDepth;
uniform sampler2D sAoNoise;

uniform float aoRadius;
uniform float aoPower;
uniform vec3 aoKernel[aoKernelSize];
uniform vec2 aoScale;

float getAoAmount(float fragDepth, vec3 fragPos, vec3 normal) {
    vec3 randomVec = texture(sAoNoise, texCoords * aoScale).xyz;
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    float occlusion = 0;
    for (int i = 0; i < aoKernelSize; i++) {
        vec3 samplePos = tbn * aoKernel[i];
        samplePos = fragPos + samplePos * aoRadius;

        vec4 offset = vec4(samplePos, 1);
        offset = projectionMatrix * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = linearizeDepth(texture(sDepth, offset.xy).r, clipRange);
        float rangeCheck = smoothstep(0, 1, aoRadius / abs(fragDepth - sampleDepth));
        float occlusionFactor = step(sampleDepth, -samplePos.z) * rangeCheck;
        occlusion += occlusionFactor;
    }

    return pow(occlusion / float(aoKernelSize), aoPower);
}

void main() {
    vec3 worldPos = texture(sPosMaterial, texCoords).xyz;
    vec3 worldNormal = texture(sNormal, texCoords).xyz;

    vec3 ssPos = (viewMatrix * vec4(worldPos, 1)).xyz;
    vec3 ssNormal = inverse(transpose(mat3(viewMatrix))) * worldNormal;
    float fragDepth = linearizeDepth(texture(sDepth, texCoords).r, clipRange);

    if (fragDepth >= clipRange.y) aoTex = 0;
    else aoTex = 1 - getAoAmount(fragDepth, ssPos, ssNormal)*2;
}
