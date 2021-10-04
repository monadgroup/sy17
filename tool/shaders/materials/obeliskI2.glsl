
EvaluatedMaterial obeliskI2Material(MaterialInput val) {
    float curveOffset = val.p.y*20 + noise(val.p*30)*3;
    float curve = sin(time/4+curveOffset)-0.6;
    float mixAmt = 1;//saturate(5 * curve);

    EvaluatedMaterial obeliskMat = obeliskMaterial(val);
    EvaluatedMaterial baseMat = EvaluatedMaterial(
        val.n,
        vec3(0),
        vec3(1, 0.2, 0.2) * 2,
        0,
        0.4,
        0,
        1
    );
    return materialMix(obeliskMat, baseMat, mixAmt);
}
