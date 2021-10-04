float getFogAmount(float d) {
    return pow(saturate(d / clipRange.y), 3);
}

vec3 applyFog(vec3 color, vec3 fogColor, float d) {
    return mix(color, fogColor, pow(saturate(d / clipRange.y), 2));
}

void main() {
    vec4 posMaterial = texture(sPosMaterial, texCoords);
    vec3 n = texture(sNormal, texCoords).rgb;
    vec3 localP = texture(sLocalPos, texCoords).rgb;
    float depth = linearizeDepth(texture(sDepth, texCoords).r, clipRange);

    vec3 p = posMaterial.xyz;
    float mat = posMaterial.w;
    vec3 d = getRay(texCoords, viewMatrix, projectionMatrix).d;

    vec3 finalColor;
    vec3 glowColor = vec3(0);

    if (mat == 0) {
        finalColor = getSky(d, cameraPos.xyz);
    } else {
        MaterialInput val = MaterialInput(depth, d, localP, n);
        EvaluatedMaterial mat = evaluateMaterial(mat, val);

        float fogAmt = getFogAmount(distance(cameraPos.xyz, p));
        finalColor = lightScene(mat, p, d, n, depth) + mat.emissive;
        finalColor = mix(finalColor, getFog(d, cameraPos.xyz), fogAmt);

        glowColor = mat.emissive * (1 - fogAmt);
    }

    outColor = max(vec3(0), finalColor);

    float brightness = dot(outColor, vec3(0.2126, 0.7152, 0.0722));
    outBloom = max(vec3(0), step(bloomThreshold, brightness) * outColor + glowColor);
}
