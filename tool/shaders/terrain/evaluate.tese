#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/materials.glsl"
#include "../common/camera_data.glsl"
#include "../common/mapNormal.glsl"
#include "../marsHeightmap.glsl"

layout(quads, equal_spacing, ccw) in;

out vec3 vPosition;
out float vDepth;

void main() {
    vec3 p0 = mix
    (gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_TessCoord.x);
    vec3 p1 = mix(gl_in[3].gl_Position.xyz, gl_in[2].gl_Position.xyz, gl_TessCoord.x);
    vec3 currentPos = mix(p0, p1, gl_TessCoord.y);

    float height = marsHeightmap(currentPos.xz);

    vPosition = currentPos + vec3(0, height, 0);
    gl_Position = projectionMatrix * viewMatrix * vec4(vPosition, 1);
    vDepth = 0.5 + 0.5 * (gl_Position.z / gl_Position.w);
}
