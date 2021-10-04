layout(location=0) in vec3 Position;

uniform vec2 patchRepeats;
uniform vec2 patchSize;
uniform vec2 followPos;

void main() {
    vec2 cameraOffset = floor(followPos.xy / patchSize) * patchSize;

    vec2 patchPos = vec2(patchSize.x * mod(gl_InstanceID, patchRepeats.x), patchSize.y * floor(gl_InstanceID / patchRepeats.x));
    patchPos -= patchRepeats * patchSize / 2;
    vec3 shiftedPos = Position + vec3(patchPos.x, 0, patchPos.y);
    shiftedPos += vec3(cameraOffset.x, 0, cameraOffset.y);
    gl_Position = vec4(shiftedPos, 1);
}
