
EvaluatedMaterial laserMaterial(MaterialInput val) {
    float laserAmount = fbm(val.p.x*10 + time/5);

    return EvaluatedMaterial(
        val.n,
        vec3(0),
        vec3(0.2, 0.2, 1) * (2 + laserAmount * 6),
        0,
        0,
        0,
        0
    );
}
