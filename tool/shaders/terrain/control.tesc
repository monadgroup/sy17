#include "../common/camera_data.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/mapNormal.glsl"
#include "../marsHeightmap.glsl"

layout(vertices=4) out;

uniform ivec2 viewportSize;
uniform float tesselMode;

float tessellatedTriWidth = 5;

float sphereToScreenSpaceTessellation(vec3 cp0, vec3 cp1, float diameter) {
    if (tesselMode <= 0) {
        vec3 mid = (cp0 + cp1) * 0.5;

        vec4 p0 = viewMatrix * vec4(mid, 1.0);
        vec4 p1 = p0;
        p1.x += diameter;

        vec4 clip0 = projectionMatrix * p0;
        vec4 clip1 = projectionMatrix * p1;

        clip0 /= clip0.w;
        clip1 /= clip1.w;

        clip0.xy = ((clip0.xy * 0.5) + 0.5) * viewportSize;
        clip1.xy = ((clip1.xy * 0.5) + 0.5) * viewportSize;

        return distance(clip0, clip1) / tessellatedTriWidth;
	}

	float dist0 = distance(cameraPos.xyz, cp0);
	float dist1 = distance(cameraPos.xyz, cp1);

	float tes0 = max(4, 64 - dist0*5);
	float tes1 = max(4, 64 - dist1*5);

	return mix(tes0, tes1, 0.5);
}

void main() {
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 p3 = gl_in[3].gl_Position.xyz;

    mat4 m = projectionMatrix * viewMatrix;
    float sideLen = p1.x - p0.x;
    gl_TessLevelOuter[0] = sphereToScreenSpaceTessellation(p3, p0, sideLen);
    gl_TessLevelOuter[1] = sphereToScreenSpaceTessellation(p0, p1, sideLen);
    gl_TessLevelOuter[2] = sphereToScreenSpaceTessellation(p1, p2, sideLen);
    gl_TessLevelOuter[3] = sphereToScreenSpaceTessellation(p2, p3, sideLen);

    gl_TessLevelInner[0] = (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]) * 0.5;
    gl_TessLevelInner[1] = (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]) * 0.5;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
