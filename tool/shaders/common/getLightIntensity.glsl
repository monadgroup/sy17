const float kMinVariance = 0.0001;
const float kShadowAmount = 1;
const float kOffset = 0.8;

float chebyshevUpperBound(sampler2D lightDepth, vec3 coords) {
    //coords.z = coords.z * 0.5 + 0.5;
    vec2 moments = texture(lightDepth, coords.xy * 0.5 + 0.5).rg;

    // if fragment is in front of occluder, we are fully lit
    if (coords.z <= moments.r) return 1;

    float variance = moments.g - pow(moments.r, 2);
    variance = max(variance, kMinVariance);

    float d = coords.z - moments.r;
    float softAmt = variance / (variance + d * d);

    //return saturate((softAmt - kOffset) / (8*d) + kOffset);
    return softAmt;
}

float getLightIntensity(sampler2D lightDepth, vec3 worldP) {
    vec4 shadowCoord = lightMatrix * vec4(worldP, 1);
    vec3 postWCoord = shadowCoord.xyz / shadowCoord.w;
    if (abs(postWCoord.x) >= 1 || abs(postWCoord.y) >= 1) return 1;
    float pShadow = chebyshevUpperBound(lightDepth, postWCoord);
    return 1.0 - kShadowAmount + kShadowAmount * pShadow;
}
