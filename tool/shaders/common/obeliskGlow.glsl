
#define GLOW_MODE_ON 0
#define GLOW_MODE_DOWN 1
#define GLOW_MODE_SPIN 2
#define GLOW_MODE_OFF 3
#define GLOW_MODE_EXPLODE 4
#define GLOW_MODE_FILL 5

uniform float laserProgress;

EvaluatedMaterial obeliskGlow(float mode, vec3 color, float multiplier, float amount, MaterialInput val) {
    float mixAmt = 0;

    if (mode <= GLOW_MODE_ON) mixAmt = 1;
    else if (mode <= GLOW_MODE_SPIN) {
        float curveOffset;
        float timeDivisor = 4;

        if (mode <= GLOW_MODE_DOWN) {
            curveOffset = -val.p.y*20*multiplier + noise(val.p*30*multiplier)*3;
        } else {
            curveOffset = atan(val.p.x, val.p.z);
            timeDivisor = 2;
        }

        float curve = sin(time/timeDivisor + curveOffset)-0.6;
        mixAmt = saturate(5 * curve) * step(laserProgress, val.p.x / clipRange.y);
    } else if (mode <= GLOW_MODE_EXPLODE) {
        float hitDist = min(
            distance(val.p, vec3(-48.8, 1.1, 40.28) - vec3(-50, 0, 40.73)),
            distance(val.p, vec3(-48.79, 1.11, 40.78) - vec3(-50, 0, 40.73))
        );
        float minDist = (time - 1533)/5;
        mixAmt = step(hitDist, minDist);
    } else if (mode <= GLOW_MODE_FILL) {
        float heightDist = (time - noise(val.p*10 + vec3(time)/10)*10 - 1280)/10;
        mixAmt = smoothstep(0.8, 1., heightDist - val.p.y) * saturate((noise(val.p*10 + vec3(time)/10)-0.5)*2+0.5);

        float hitDist = min(
            distance(val.p, vec3(-48.8, 1.1, 40.28) - vec3(-50, 0, 40.73)),
            distance(val.p, vec3(-48.79, 1.11, 40.78) - vec3(-50, 0, 40.73))
        );
        float minDist = (time - 1472)/5;
        float curveOffset = atan(val.p.x, val.p.z);
        float curve = sin(time/2 + curveOffset)-0.6;
        float spinColorAmt = mix(1, saturate(5 * curve), smoothstep(1600, 1610, time));

        color = mix(color, vec3(0.2, 0.2, 1)*2*spinColorAmt, step(hitDist, minDist));
        mixAmt = mix(mixAmt, 1, step(hitDist, minDist));
    }


    mixAmt *= amount;

    EvaluatedMaterial obeliskMat = obeliskMaterial(val);
    EvaluatedMaterial baseMat = EvaluatedMaterial(
        val.n,
        vec3(0),
        mix(vec3(0), color * 2, mixAmt),
        0,
        0.4,
        0,
        1
    );
    return materialMix(obeliskMat, baseMat, mix(1, mixAmt, obeliskDirtiness));
}
