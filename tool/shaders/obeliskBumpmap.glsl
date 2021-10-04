
float obeliskBumpmap(vec2 p) {
    return saturate(fbm(p * 100) + 0.8);
}

vec3 obeliskBumpmapNormal(vec2 p) {
    MAP_NORMAL(obeliskBumpmap, 0.0005);
}
