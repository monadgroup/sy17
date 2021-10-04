
EvaluatedMaterial obeliskI1Material(MaterialInput val) {

    float curveOffset = atan(val.p.x, val.p.z); //val.p.y*20 + noise(val.p*30)*3;
    float curveVal = time;

    float curve = sin(curveVal/4+curveOffset)-0.6;
    float mixAmt = saturate(5 * curve);

    EvaluatedMaterial obeliskMat = obeliskMaterial(val);
    EvaluatedMaterial baseMat = EvaluatedMaterial(
        val.n,
        vec3(0),
        vec3(0.2, 0.2, 1) * 2,
        0,
        0.4,
        0,
        1
    );
    return materialMix(obeliskMat, baseMat, mixAmt);
}
