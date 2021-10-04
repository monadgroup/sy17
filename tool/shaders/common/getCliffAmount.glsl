
float getCliffAmount(vec3 p) {
    vec3 largeNormal = largeMarsHeightmapNormal(p.xz);
    return saturate(2 - pow(largeNormal.y, 10) * 4);
}
