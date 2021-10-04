
float foilBumpmap(vec2 p) {
    return pow(fbm(p * 5), 1);
}

vec3 foilBumpmapNormal(vec2 p) {
    MAP_NORMAL(foilBumpmap, 0.001);
}
