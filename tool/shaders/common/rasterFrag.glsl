out vec4 outPosMaterial;
out vec3 outNormal;
out vec3 outLocalPos;
out vec3 outVelocity;
out vec2 outVariance;

void rasterFrag(vec3 VertPos, vec3 VertVelocity, vec3 VertNormal, vec3 VertLocalPos, float VertMaterial, float VertDepth) {
    outPosMaterial = vec4(VertPos, VertMaterial);
    outNormal = normalize(VertNormal);
    outLocalPos = VertLocalPos;
    outVelocity = VertVelocity;

    VertDepth = VertDepth * 2 - 1;
    float dx = dFdx(VertDepth);
    float dy = dFdy(VertDepth);
    outVariance = vec2(VertDepth, VertDepth*VertDepth + 0.25*(dx*dx + dy*dy));
}
