vec3 raymarchNormal(vec3 p) {
    vec2 e = vec2(MIN_RADIUS, -MIN_RADIUS);
    vec4 o = vec4(
        fScene(p + e.xyy).dist,
        fScene(p + e.yyx).dist,
        fScene(p + e.yxy).dist,
        fScene(p + e.xxx).dist
    );
    return normalize(o.wzy + o.xww - o.zxz - o.yyx);
}
