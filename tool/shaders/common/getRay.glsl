struct Ray {
    vec3 o;
    vec3 d;
};

vec3 getRay_pos(vec4 clipSpacePosition, mat4 invViewMatrix, mat4 invProjMatrix) {
    vec4 viewSpacePosition = invProjMatrix * clipSpacePosition;
    viewSpacePosition /= viewSpacePosition.w;
    vec4 worldSpacePosition = invViewMatrix * viewSpacePosition;
    return worldSpacePosition.xyz;
}

Ray getRay(vec2 coords, mat4 viewMatrix, mat4 projectionMatrix) {
    mat4 inView = inverse(viewMatrix);
    mat4 inProj = inverse(projectionMatrix);

    vec3 p0 = getRay_pos(vec4(coords * 2 - 1, 0, 1), inView, inProj);
    vec3 p1 = getRay_pos(vec4(coords * 2 - 1, 1, 1), inView, inProj);

    return Ray(p0, normalize(p1 - p0));
}
