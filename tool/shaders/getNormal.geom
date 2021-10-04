#include "common/camera_data.glsl"
#include "common/writePoint.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 vPosition[];
in float vMaterial[];

void main() {
    mat4 useMatrix = projectionMatrix * viewMatrix;

    vec3 u = vPosition[2] - vPosition[0];
    vec3 v = vPosition[1] - vPosition[0];
    vec3 n = normalize(cross(u, v));

    writePoint(useMatrix, vPosition[0], n, vMaterial[0]);
    writePoint(useMatrix, vPosition[1], n, vMaterial[1]);
    writePoint(useMatrix, vPosition[2], n, vMaterial[2]);
}
