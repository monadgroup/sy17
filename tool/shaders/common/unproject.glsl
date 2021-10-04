
vec3 unproject(vec3 coords, vec2 clipRange, mat4 viewMatrix, mat4 projectionMatrix) {
    float d = coords.z * 2 - 1;//linearizeDepth(coords.z, clipRange);
    return getRay_pos(vec4(coords.xy * 2 - 1, d, 1), inverse(viewMatrix), inverse(projectionMatrix));
}
