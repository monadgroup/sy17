#include "../common/rover_data.glsl"

uniform sampler2D sTrack;
uniform vec2 trackSize;
uniform float craterSpread;
uniform float groundWidth;

EvaluatedMaterial terrainMaterial(MaterialInput val) {
    vec3 deepRed = mix(vec3(0.459, 0.048, 0.019)*2, vec3(0.5), 0.2);
    vec3 orange = mix(vec3(0.715, 0.089, 0.036)*2, vec3(0.5), 0.2);
    float variation = pow(saturate(fbm(val.p)*2), 3);
    vec3 color = mix(orange, deepRed, variation);

    float speckle = (noise(val.p*1000) + noise(val.p*2000)) / 2;

    vec2 roverRelPos = (roverPos - val.p.xz) / trackSize + vec2(0.5);
    float mult = step(0, roverRelPos.x) * step(roverRelPos.x, 1) * step(0, roverRelPos.y) * step(roverRelPos.y, 1);
    float trackAmount = step(0.3, texture(sTrack, roverRelPos).r) * mult * 0.5;

    EvaluatedMaterial groundMat = EvaluatedMaterial(
        val.n,
        mix(mix(color, color/2, speckle), vec3(0), trackAmount),
        vec3(0),
        0,
        1,
        0,
        1
    );

    vec3 cliffOne = vec3(0.2, 0.05, 0.1)/2;
    vec3 cliffTwo = deepRed/2;
    float cliffVariation = pow(saturate(fbm(val.p*10)), 2);

    float patchAmt = pow(fbm(val.p)*1.2, 2);
    patchAmt = sin(M_PI * (patchAmt - 0.5)) * 0.5 + 0.5;

    float cliffAmount = saturate(getCliffAmount(val.p) + patchAmt);

    EvaluatedMaterial cliffMat = EvaluatedMaterial(
        val.n,
        mix(cliffOne, cliffTwo, cliffVariation),
        vec3(0),
        0,
        0.5 + cliffVariation * 0.5,
        0,
        1
    );
    EvaluatedMaterial normalMat = materialMix(groundMat, cliffMat, cliffAmount);

    float noiseVal = noise(val.p*10 + vec3(0, time/5, 0));
    float blueAmt = smoothstep(0.5, 0.6, noiseVal) * (1 - smoothstep(0.6, 0.7, noiseVal));
    float craterDist = distance(val.p.xz, craterPos);
    float repeatDist = mod(craterDist/2 + time/50, 1)/craterSpread;
    float craterEdgeMix = step((1 - groundWidth)/craterSpread, repeatDist);
    EvaluatedMaterial craterMat = EvaluatedMaterial(
        val.n,
        vec3(0, 0, 0),
        vec3(0.5, 0.1, 1) * blueAmt,
        0,
        0.5,
        0,
        1
    );

    return materialMix(normalMat, craterMat, craterEdgeMix * craterSpread);
}
