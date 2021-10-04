#include "../obeliskBumpmap.glsl"

uniform float obeliskDirtiness;

EvaluatedMaterial obeliskMaterial(MaterialInput val) {
    vec3 c1 = vec3(0.8, 0.8, 1)/10;
    vec3 c2 = vec3(1, 0.8, 0.8)/5;

    vec3 pickP = val.p*15;

    float colorPick = smoothstep(0.45, 0.55, noise(floor(pickP)));
    float aAmount = 1 - saturate(2 * colorPick);
    float bAmount = saturate(2 * colorPick - 1);
    float cAmount = 1 - aAmount - bAmount;

    vec3 pickColor = c1 * aAmount + c2 * bAmount + vec3(0) * cAmount;

    float stain = saturate(fbm(val.p*6.67));

    vec3 bump = BUMP_3D(obeliskBumpmapNormal, val.p, val.n) * obeliskDirtiness;

    return EvaluatedMaterial(
        normalize(val.n + bump),
        mix(c1, pickColor * (1 - stain), obeliskDirtiness),
        vec3(0),
        0,
        1,
        0,
        1
    );
}
