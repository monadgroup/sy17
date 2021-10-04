out vec3 VertPos;
out vec3 VertNormal;
out vec3 VertLocalPos;
out float VertMaterial;
out float VertDepth;

void writePoint(mat4 useMatrix, vec3 p, vec3 n, float material) {
    gl_Layer = gl_InvocationID;
    gl_Position = projectionMatrix * viewMatrix * vec4(p, 1);
    VertPos = p;
    VertNormal = n;
    VertLocalPos = p;
    VertMaterial = material;
    VertDepth = 0.5 + 0.5 * (gl_Position.z / gl_Position.w);
    EmitVertex();
}
