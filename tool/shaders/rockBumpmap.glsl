
float rockBumpmap(vec2 p) {
    return saturate(pow(fbm(p*100) + fbm(p*50), 5));
}

vec3 rockBumpmapNormal(vec2 p) {
    MAP_NORMAL(rockBumpmap, 0.001);
}
