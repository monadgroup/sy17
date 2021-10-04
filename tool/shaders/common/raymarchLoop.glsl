#define MAX_ITERATIONS 80
//#define USE_ENHANCED_TRACING

RaymarchResult raymarchLoop(inout vec3 p, inout float t, vec3 d, float pixelRadius, float clipFar) {
#ifdef USE_ENHANCED_TRACING
    bool forceHit = false;
    float omega = 1.2;
    float candidate_error = INFINITY;
    float candidate_t = t;
    vec3 o = p;
    vec3 candidate_p = o;
    RaymarchResult candidate_m = RaymarchResult(vec3(0), 0, 0);
    float previousRadius = 0;
    float stepLength = 0;
    float functionSign = sgn(fScene(o).dist);

    int i = 0;
    for (i = 0; i < MAX_ITERATIONS; ++i) {
        p = d*t + o;
        RaymarchResult mat = fScene(p);
        float signedRadius = functionSign * mat.dist;
        float radius = abs(signedRadius);

        bool sorFail = omega > 1 && (radius + previousRadius) < stepLength;
        if (sorFail) {
            stepLength -= omega * stepLength;
            omega = 1;
        } else {
            stepLength = signedRadius * omega;
        }

        previousRadius = radius;
        float error = radius / t;

        if (!sorFail && error < candidate_error) {
            candidate_m = mat;
            candidate_t = t;
            candidate_p = p;
            candidate_error = error;
        }
        if (!sorFail && error < pixelRadius || t > clipFar) {
            break;
        }
        t += stepLength;
    }

    if ((t > clipFar || candidate_error > pixelRadius) && !forceHit) {
        return RaymarchResult(o + clipFar * d, 0, 0);
    }

    p = candidate_p;
    t = candidate_t;
    return candidate_m;
#else
    vec3 o = p;
    int i = 0;
    for (i = 0; i < MAX_ITERATIONS && t < clipFar; ++i) {
        p = d*t + o;
        RaymarchResult mat = fScene(p);
        if (mat.dist < MIN_RADIUS) {
            return mat;
        }
        t += mat.dist;
    }
    p = o + clipFar * d * 2; // make sure we're out of range
    return RaymarchResult(p, 0, 0);
#endif
}
