
uniform float shipAmount;
uniform float rocketAmount;

EvaluatedMaterial rocketMaterial(MaterialInput val) {
    vec3 grid = smoothstep(0.45, 0.55, abs(0.5 - mod(val.p*5, 1)));
    float gridAmt = 1 - (1 - grid.x) * (1 - grid.y) * (1 - grid.z);

    vec3 rocketColorA = vec3(1);
    vec3 rocketColorB = vec3(1, 1, 0.1);
    vec3 rocketColor = mix(rocketColorA, rocketColorB, rand(time))*5;
    float rocketColorAmt = saturate((val.p.y+1)*10);

    return EvaluatedMaterial(
        val.n,
        vec3(0),
        mix(vec3(1, 0.15, 0.8) * gridAmt * 4, rocketColor, (1 - rocketColorAmt) * rocketAmount) * shipAmount,
        0,
        0.2,
        0,
        1
    );
}
