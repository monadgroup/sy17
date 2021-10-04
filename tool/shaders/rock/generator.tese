#include "../common/camera_data.glsl"
#include "../common/materials.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"

layout(quads, equal_spacing, ccw) in;

patch in mat3 tModelMatrix;
patch in float tRockId;

out vec3 vPosition;
out float vRockId;

void main() {
    float angleHorizontal = gl_TessCoord.x * M_PI*2;
    float angleVertical = gl_TessCoord.y * M_PI - M_PI/2;
    vec3 d = vec3(cos(angleVertical) * cos(angleHorizontal), sin(angleVertical), cos(angleVertical) * sin(angleHorizontal));
    vec3 offsetPos = d * fbm(d + vec3(tRockId));
    vec3 transformedPos = tModelMatrix * offsetPos;
    transformedPos.y = pow(max(0, transformedPos.y*0.8 + 0.3), 2);

    vPosition = transformedPos;
    vRockId = tRockId;
}
