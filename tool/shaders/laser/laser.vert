#include "../common/math.glsl"
#include "../common/object_data.glsl"
#include "../common/camera_data.glsl"
#include "../common/materials.glsl"
#include "../common/rasterVert.glsl"

layout(location=0) in vec3 inPos;

uniform vec2 obeliskGridSize;
uniform float laserProgress;

void main() {
    vec3 p = inPos * vec3(clipRange.y*2, 0.05, 1);

    vec3 movePos = vec3(0, 1.1, gl_InstanceID * obeliskGridSize.x/4 - obeliskGridSize.x/8);

    vec3 relPos = vec3(laserProgress, 0, 40.53) + movePos;
    vec3 relCamPos = cameraPos.xyz - relPos;

    float yRot = -atan(relCamPos.y, relCamPos.z);
    mat3 rotMatrix = mat3(
        vec3(1, 0, 0),
        vec3(0, cos(yRot), sin(yRot)),
        vec3(0, -sin(yRot), cos(yRot))
    );

    vec3 newPos = rotMatrix * p + relPos;
    vec3 n = rotMatrix * vec3(0, 0, 1);

    rasterVert(newPos, newPos, n, MATERIAL_LASER - 0.5, mat4(1));
}
