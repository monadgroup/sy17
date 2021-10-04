#include "../common/camera_data.glsl"
#include "../common/math.glsl"
#include "../common/noise.glsl"
#include "../common/mapNormal.glsl"

out float vScaleFactor;
out mat3 vModelMatrix;
out float vRockId;

uniform float rockSize;

void main() {
    float rotateAngle1 = rand(gl_InstanceID) * M_PI * 2;
    float rotateAngle2 = rand(gl_InstanceID + 5985) * M_PI * 2;
    float rotateAngle3 = rand(gl_InstanceID + 309) * M_PI * 2;
    vScaleFactor = (noise(gl_InstanceID + 658) + 0.1) * rockSize;
    vec3 scaleAmount = vec3(
        noise(gl_InstanceID + 43) * 0.7 + 0.3,
        pow(noise(gl_InstanceID + 609), 1) * 0.8 + 0.2,
        noise(gl_InstanceID + 348) * 0.7 + 0.3
    ) * vScaleFactor;
    mat3 rotateMat = mat3(cos(rotateAngle2), 0, -sin(rotateAngle2), 0, 1, 0, sin(rotateAngle2), 0, cos(rotateAngle2)) * mat3(1, 0, 0, 0, cos(rotateAngle3), sin(rotateAngle3), 0, -sin(rotateAngle3), cos(rotateAngle3)) * mat3(cos(rotateAngle1), sin(rotateAngle1), 0, -sin(rotateAngle1), cos(rotateAngle1), 0, 0, 0, 1);
    mat3 scaleMat = mat3(scaleAmount.x, 0, 0, 0, scaleAmount.y, 0, 0, 0, scaleAmount.z);

    vModelMatrix = scaleMat * rotateMat;
    vRockId = gl_InstanceID;
}
