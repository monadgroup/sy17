#include "common/math.glsl"
#include "common/object_data.glsl"
#include "common/camera_data.glsl"
#include "common/scene_data.glsl"
#include "common/rasterVert.glsl"
#include "common/noise.glsl"
#include "common/mapNormal.glsl"
#include "marsHeightmap.glsl"

// vertex data
layout(location=0) in vec3 inPos;
layout(location=1) in vec3 inNormal;
layout(location=2) in float inMaterial;

uniform vec2 coord;
uniform float offsetAmount;
uniform float flyAmount;
uniform float wobbleAmount;

vec2 gridSize = vec2(2, 0.5);

vec4 getQuaternion(vec3 axis, float angle) {
    return vec4(axis * sin(angle/2), cos(angle/2));
}

vec4 slerp(vec4 q0, vec4 q1, float t) {
    float d = dot(q0, q1);

    if (abs(d) > 0.9995) {
        return normalize(q0 + t*(q1 - q0));
    }

    if (d < 0) {
        q1 = -q1;
        d = -d;
    }

    clamp(d, -1, 1);
    float theta_0 = acos(d);
    float theta = theta_0 * t;

    vec4 q2 = normalize(q1 - q0*d);
    return q0*cos(theta) + q2*sin(theta);
}

vec4 mul(vec4 x, vec4 y) {
    return vec4(
        x.x * y.x - x.y * y.y - x.z * y.z - x.w * y.w,
        x.x * y.y + x.y * y.x - x.z * y.w + x.w * y.z,
        x.x * y.z + x.y * y.w + x.z * y.x - x.w * y.y,
        x.x * y.w - x.y * y.z + x.z * y.y + x.w * y.x
    );
}

void main() {
    vec2 realCoord = (coord * gridSize).xy * vec2(-1, -1) + vec2(8.5, 40.78);
    realCoord.x++;

    vec2 c = coord + vec2(1, 0);

    float height = marsHeightmap(realCoord)-0.05;
    if (coord.x == 1) height--;

    float flyHeight = 0.8;
    height = mix(height, flyHeight, flyAmount);

    vec3 translation = vec3(realCoord.x, height, realCoord.y);
    vec3 baseTranslation = translation;

    translation += vec3(
        fbm(vec3(coord, time/100)),
        fbm(vec3(time/100, coord)),
        fbm(vec3(coord.x, time/100, coord.y))
    )/5 * flyAmount * wobbleAmount;

    // rotate randomly
    float rotateAngle1 = rand(c) * M_PI * 2;
    float rotateAngle2 = rand(c + vec2(5985)) * M_PI * 2;
    float rotateAngle3 = rand(c + vec2(309)) * M_PI * 2;

    vec4 rot = getQuaternion(vec3(0, 0, 1), rotateAngle1);
    rot = mul(rot, getQuaternion(vec3(1, 0, 0), rotateAngle2));
    rot = mul(rot, getQuaternion(vec3(0, 1, 0), rotateAngle3));

    vec4 q = slerp(normalize(rot), getQuaternion(vec3(0), 0), offsetAmount);
    mat3 rotateMat = mat3(1) + 2*mat3(
        vec3(-q.y*q.y - q.z*q.z, q.x*q.y, q.x*q.z),
        vec3(q.x*q.y, -q.x*q.x - q.z*q.z, q.y*q.z),
        vec3(q.x*q.z, q.y*q.z, -q.x*q.x - q.y*q.y)
    ) + 2 * q.w * mat3(
        vec3(0, q.z, -q.y),
        vec3(-q.z, 0, q.x),
        vec3(q.y, -q.x, 0)
    );

    mat4 translateMat = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(translation, 1));

    rasterVert(inPos, baseTranslation + inPos, inNormal, inMaterial, translateMat * mat4(rotateMat));
}
