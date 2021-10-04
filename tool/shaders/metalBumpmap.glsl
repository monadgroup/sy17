
float metalBumpmap(vec2 p) {
    return pow(abs(fbm(p * 50)*2 - 1), 0.5);
}

vec3 metalBumpmapNormal(vec2 p) {
    MAP_NORMAL(metalBumpmap, 0.001);
}
