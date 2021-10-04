float barAmt(float x, float width) {
    return max(0, 1 - abs(width * mod(x, 0.1) - 1));
}

EvaluatedMaterial solarMaterial(MaterialInput val) {
    val.p *= 40;
    float colorChange = fbm(val.p/2);
    vec3 color = mix(vec3(0, 0.01, 0.01), vec3(0, 0.1, 0.4), colorChange);

    float barIndex = barAmt(val.p.z, 200)/3 + barAmt(val.p.z / 10, 600);
    color = mix(color, vec3(1), barIndex);

    float dirtAmt = saturate(pow(fbm(val.p.yzx/3)*2,2));
    color *= 1 - min(dirtAmt, 0.8);

    EvaluatedMaterial solarMat = EvaluatedMaterial(
        val.n,
        color,
        vec3(0),
        0,
        1 - dirtAmt,
        0.3,
        1
    );

    return solarMat;
}
