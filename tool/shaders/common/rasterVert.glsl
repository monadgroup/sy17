out vec3 VertPos;
out vec3 VertNormal;
out vec3 VertLocalPos;
out float VertMaterial;
out float VertDepth;

void rasterVert(vec3 p, vec3 localP, vec3 n, float m, mat4 mm) {
    vec4 worldPos = mm * vec4(p, 1);
    gl_Position = projectionMatrix * viewMatrix * worldPos;

    VertPos = worldPos.xyz;
    VertNormal = inverse(transpose(mat3(mm))) * n;
    VertLocalPos = localP;
    VertMaterial = m;
    VertDepth = 0.5 + 0.5 * (gl_Position.z / gl_Position.w);
}
