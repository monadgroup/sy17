#include "../common/camera_data.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/mapNormal.glsl"
#include "../marsHeightmap.glsl"
#include "../common/rasterVert.glsl"
#include "../common/materials.glsl"
#include "../common/rover_data.glsl"

layout(location=0) in vec3 vPosition;

void main() {
    vec3 newPos = vPosition + vec3(
        roverPos.x,
        marsHeightmap(roverPos),
        roverPos.y
    );

    rasterVert(newPos, newPos, vec3(0), MATERIAL_WHITE, mat4(1));

    //gl_Position = projectionMatrix * viewMatrix * vec4(newPos, 1);
}
