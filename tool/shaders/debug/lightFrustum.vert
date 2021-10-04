#include "../common/light_data.glsl"
#include "../common/camera_data.glsl"

in vec3 vPosition;
out vec3 VertPos;

void main() {
    vec4 worldPos = inverse(lightMatrix) * vec4(vPosition, 1);
    worldPos /= worldPos.w;
    VertPos = worldPos.xyz;

    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
