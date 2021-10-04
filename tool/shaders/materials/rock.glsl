#include "../rockBumpmap.glsl"

EvaluatedMaterial rockMaterial(MaterialInput val) {
    vec3 colorA = vec3(0.205, 0.117, 0.087)*5;
    vec3 colorB = vec3(0.715, 0.089, 0.036)*3;

    vec3 groundColor = mix(vec3(0.715, 0.089, 0.036)*2, vec3(0.5), 0.2);
    vec3 cliffColor = vec3(0.2, 0.05, 0.1)/2;
    //vec3 groundColor = mix(groundColorA, groundColorB, getCliffAmount(val.p));

    float variation = saturate(pow(fbm(val.p*10)*2, 5));

    float heightDiff = saturate(abs(val.p.y - marsHeightmap(val.p.xz)) * 200);

    /*vec3 xBump = rockBumpmapNormal(val.p.yz).yxz;
    vec3 yBump = rockBumpmapNormal(val.p.xz).xyz;
    vec3 zBump = rockBumpmapNormal(val.p.xy).xzy;

    vec3 bump = normalize(xBump * val.n.x + yBump * val.n.y + zBump * val.n.z);*/
    vec3 bump = BUMP_3D(rockBumpmapNormal, val.p, val.n);

    vec3 pickColor = mix(groundColor, mix(colorB, colorA, variation), heightDiff);
    pickColor = mix(pickColor, cliffColor, getCliffAmount(val.p) / 3 * 2);

    return EvaluatedMaterial(
        normalize(val.n + bump * 0.3),
        pickColor,
        vec3(0),
        0,
        0.9,
        1,
        1
    );
}
