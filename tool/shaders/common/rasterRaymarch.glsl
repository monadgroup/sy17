uniform vec2 windowSize;

#include "../common/raymarchNormal.glsl"
#include "../common/raymarchLoop.glsl"

void rasterRaymarch(float pDivisor, vec3 velocity) {
    Ray r = getRay(gl_FragCoord.xy / windowSize, viewMatrix, projectionMatrix);

    vec3 d = r.d;
    vec3 o = r.o;
    float pixelSize = 2 * sin(radians(cameraPos.w)) / windowSize.x;
    float t = 0;
    RaymarchResult res = raymarchLoop(o, t, d, pixelSize, clipRange.y);

    //outPosMaterial = vec4(o, res.material);
    //outNormal = raymarchNormal(o);

    float far = gl_DepthRange.far; float near = gl_DepthRange.near;
    vec4 clipSpacePos = projectionMatrix * viewMatrix * vec4(o, 1);
    float ndc_depth = clipSpacePos.z / clipSpacePos.w;
    float depth = (((far - near) * ndc_depth) + near + far) / 2;
    gl_FragDepth = saturate(depth);

    rasterFrag(o, velocity, raymarchNormal(o), res.localP/pDivisor, res.material, 0.5 + 0.5 * (clipSpacePos.z / clipSpacePos.w));
}
